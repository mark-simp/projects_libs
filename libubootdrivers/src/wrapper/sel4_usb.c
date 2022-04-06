#include <sel4platsupport/io.h>
#include <platsupport/delay.h>

#include <libfdt.h>
#include <uboot_wrapper.h>
#include <sel4_timer.h>
#include <utils/page.h>
#include <sel4_dma_wrapper.h>

void* uboot_fdt_pointer = NULL;


int get_node_size_and_address_data(uintptr_t *addr, size_t *size, int *addr_cells, int *size_cells, int node_offset) {
    // Get the name of the path for logging purposes.
    const char *path_name = fdt_get_name(uboot_fdt_pointer, node_offset, NULL);

    // Get the offset of the parent node (need to query for cell sizes).
    int parent_node_offset = fdt_parent_offset(uboot_fdt_pointer, node_offset);
    if (parent_node_offset < 0) {
        ZF_LOGD("Unable to find parent of path '%s' in device tree.", path_name);
        return node_offset;
    }

    // Read the number of address and size cells used (from the parent node).
    *addr_cells = fdt_address_cells(uboot_fdt_pointer, parent_node_offset);
    if (*addr_cells < 1 || *addr_cells > 2) {
        ZF_LOGD("Path '%s' has address other than 32 or 64 bits.", path_name);
        return -1;
    }

    *size_cells = fdt_size_cells(uboot_fdt_pointer, parent_node_offset);
    if (*size_cells < 1 || *size_cells > 2) {
        ZF_LOGD("Path '%s' has size other than 32 or 64 bits.", path_name);
        return -1;
    }

    // Read the 'reg' property.
    int prop_len;
    const struct fdt_property *reg_property;
    reg_property = fdt_get_property(uboot_fdt_pointer, node_offset, "reg", &prop_len);
    if (reg_property == NULL) {
        ZF_LOGD("Path '%s' had no 'reg' property in device tree.", path_name);
        return prop_len;
    }

    // Make sure there is only a single address / size pair in the 'reg' property.
    if ((*addr_cells + *size_cells) * 4 != prop_len) {
        ZF_LOGD("Path '%s' has 'reg' property consisting of more than a single address, size pair. Unhandled.", path_name);
        return -1;
    }

    // Determine the address and size.
    if (*addr_cells == 2)
        *addr = (uintptr_t) fdt64_to_cpu(*(fdt64_t *) &reg_property->data[0]);
    else
        *addr = (uintptr_t) fdt32_to_cpu(*(fdt32_t *) &reg_property->data[0]);

    if (*size_cells == 2)
        *size = (size_t) fdt64_to_cpu(*(fdt64_t *) &reg_property->data[4 * *addr_cells]);
    else
        *size = (size_t) fdt32_to_cpu(*(fdt32_t *) &reg_property->data[4 * *addr_cells]);

    return 0;
}


int replace_physical_address_with_virtual(uintptr_t paddr, uintptr_t vaddr, size_t size, int node_offset) {

    // Get the name of the path for logging purposes.
    const char *path_name = fdt_get_name(uboot_fdt_pointer, node_offset, NULL);

    // Get the address and size of the node to be modified.
    uintptr_t node_paddr;
    size_t node_size;
    int addr_cells;
    int size_cells;
    int ret = get_node_size_and_address_data(&node_paddr, &node_size, &addr_cells, &size_cells, node_offset);
    if (0 != ret) {
        ZF_LOGD("Unable to read 'reg' property for path '%s' from device tree, ignoring.", path_name);
        return 0;
    }

    // Check to see whether the memory region is within the bounds of the mapping to be applied.
    if (!(node_paddr >= paddr && node_paddr + node_size <= paddr + size)) {
        ZF_LOGD("'reg' property for path '%s' outside of mapped virtual memory, ignoring.", path_name);
        return 0;
    }

    // Read the node's 'reg' property.
    int prop_len;
    const struct fdt_property *reg_property;
    reg_property = fdt_get_property(uboot_fdt_pointer, node_offset, "reg", &prop_len);
    if (reg_property == NULL) {
        // Note, shouldn't get here as we were previously able to read an address.
        ZF_LOGE("Path '%s' had no 'reg' property in device tree.", path_name);
        return -1;
    }

    // Work out the virtual address of this node.
    uintptr_t node_vaddr = (node_paddr - paddr) + vaddr;

    // Update the 'reg' property within the FDT with the node's virtual address.
    uint8_t tmp[prop_len];
    char *p = tmp;

    if (addr_cells == 2)
        *(fdt64_t *)p = cpu_to_fdt64(node_vaddr);
    else
        *(fdt32_t *)p = cpu_to_fdt32(node_vaddr);
    p += 4 * addr_cells;

    if (size_cells == 2)
        *(fdt64_t *)p = cpu_to_fdt64(node_size);
    else
        *(fdt32_t *)p = cpu_to_fdt32(node_size);

    ret = fdt_setprop_inplace(uboot_fdt_pointer, node_offset, "reg", tmp, prop_len);
    if (0 != ret) {
        ZF_LOGE("Unable to update device for path '%s' with device virtual address.", path_name);
        return -1;
    }
    else
        ZF_LOGD(" -> Re-mapped '%s' from paddr %p to vaddr %x.", path_name, node_paddr, node_vaddr);

    return 0;
}


int allocate_dev_resource_and_fdt_fixup(ps_io_ops_t *io_ops, const char* path) {

    // Lookup the address and size information form the memory mapped device.
    int node_offset = fdt_path_offset(uboot_fdt_pointer, path);
    if (node_offset < 0) {
        ZF_LOGE("Unable to find path '%s' in device tree.\n", path);
        return -1;
    }

    // Find the address and size of the device.
    uintptr_t paddr;
    size_t size;
    int addr_cells;
    int size_cells;
    int ret = get_node_size_and_address_data(&paddr, &size, &addr_cells, &size_cells, node_offset);
    if (0 != ret) {
        ZF_LOGE("Unable to read 'reg' property for path '%s' from device tree.", path);
        return -1;
    }

    // We now have a known and verified address and size for the device.
    // Map it into our virtual address space. Note that the address must
    // be on a 4K boundary to be accepted, this is achieved by rounding
    // the address to the nearest 4K boundary and increasing the size to
    // compensate.
    uintptr_t unaligned_paddr = paddr;
    paddr = PAGE_ALIGN_4K(paddr);
    assert(paddr <= unaligned_paddr);
    size = size + (unaligned_paddr - paddr);

    uintptr_t vaddr = (uintptr_t) ps_io_map(&io_ops->io_mapper, paddr, size, 0, PS_MEM_NORMAL);
    if (0 == vaddr) {
        ZF_LOGE("Unable to map virtual address for path '%s'.", path);
        return -1;
    }

    ZF_LOGD("Mapped '%s' of size 0x%x from paddr %p to vaddr %x.", path, size, paddr, vaddr);

    // Update the FDT to point to the virtual rather than physical address. Need to perform this
    // for all nodes that sit under the device node.
    ret = replace_physical_address_with_virtual(paddr, vaddr, size, node_offset);
    if (0 != ret) {
        ZF_LOGE("Unable to update device for path '%s' with device virtual address.", path);
        return -1;
    }
    int subnode_offset;
    fdt_for_each_subnode(subnode_offset, uboot_fdt_pointer, node_offset) {
        // Subnodes may not  have a 'reg' property so failures are expected here and ignored.
        replace_physical_address_with_virtual(paddr, vaddr, size, subnode_offset);
    }

    return 0;
}

int sel4_usb_init(ps_io_ops_t *io_ops, const char **device_paths, uint32_t device_count, const char *timer_path)
{
    int ret;

    // Start the monotonic timer.
    ret = init_and_start_timer(io_ops, timer_path);
    if (0 != ret)
        return -1;

    // Initialise the DMA management.
    sel4_dma_initialise(io_ops->dma_manager);

    // Create a copy of the FDT for U-Boot to use.
    void* orig_fdt_blob = io_ops->io_fdt.get_fn(io_ops->io_fdt.cookie);
    int fdt_size = fdt_totalsize(orig_fdt_blob);
    uboot_fdt_pointer = malloc(fdt_size);
    memcpy(uboot_fdt_pointer, orig_fdt_blob, fdt_size);

    // Allocate resources and modify addresses in the device tree for each device.
    for (int dev_index=0; dev_index < device_count; dev_index++) {
        ret = allocate_dev_resource_and_fdt_fixup(io_ops, device_paths[dev_index]);
        if (0 != ret) {
            free(uboot_fdt_pointer);
            return -1;
        }
    }

    // Start the U-Boot driver library. Provide it a pointer to the FDT blob.
    ret = initialise_uboot_drivers(uboot_fdt_pointer);
    if (ret) {
        free(uboot_fdt_pointer);
        return ret;
    }

    // Perform some U-Boot commands.

    run_uboot_command("dm tree");

    run_uboot_command("setenv stdin usbkbd"); // Use a USB keyboard as the input device

    run_uboot_command("usb start");

    run_uboot_command("dm tree");

    run_uboot_command("usb tree");

    run_uboot_command("usb info");

    // Loop for a while reading keypresses
    printf("Echoing input from the USB keyboard:\n");
    for (int x=0; x<=1000; x++) {
        while (uboot_stdin_tstc())
            printf("Received character: %c\n", uboot_stdin_getc(), stdout);
        ps_mdelay(10);
    }

    run_uboot_command("usb stop");

    shutdown_uboot_drivers();

    // All done.
    return 0;
}