/*
 * This file performs all of the initialisation and shutdown of the library
 * required from within the seL4 'world', i.e. those actions that require
 * use of seL4 libraries such as mapping of the physical memory for the
 * devices to be used.
 *
 * Following successful initialisation within this file the 'uboot_wrapper'
 * is called to continue within the U-Boot 'world'.
 */

#include <sel4platsupport/io.h>

#include <libfdt.h>
#include <uboot_wrapper.h>
#include <sel4_timer.h>
#include <utils/page.h>

void* uboot_fdt_pointer = NULL;


static int get_node_size_and_address_data(uintptr_t *addr, size_t *size, int *addr_cells, int *size_cells, int node_offset) {
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


static int allocate_dev_resource_and_fdt_fixup(const char* path) {

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

    uintptr_t vaddr = (uintptr_t) sel4_io_map_do_map(paddr, size);
    if (0 == vaddr) {
        ZF_LOGE("Unable to map virtual address for path '%s'.", path);
        return -1;
    }

    ZF_LOGD("Mapped '%s' of size 0x%x from paddr %p to vaddr %x.", path, size, paddr, vaddr);

    return 0;
}

int initialise_uboot_drivers(ps_io_ops_t *io_ops, const char **device_paths, uint32_t device_count, const char *timer_path)
{
    // Initialise the DMA management.
    sel4_dma_initialise(&io_ops->dma_manager);

    // Initialise the IO mapping management.
    sel4_io_map_initialise(&io_ops->io_mapper);

    // Start the monotonic timer.
    int ret = init_and_start_timer(io_ops, timer_path);
    if (0 != ret)
        return -1;

    // Create a copy of the FDT for U-Boot to use.
    void* orig_fdt_blob = io_ops->io_fdt.get_fn(io_ops->io_fdt.cookie);
    int fdt_size = fdt_totalsize(orig_fdt_blob);
    uboot_fdt_pointer = malloc(fdt_size);
    if (uboot_fdt_pointer == NULL) {
        shutdown_timer();
        return -ENOMEM;
    }
    memcpy(uboot_fdt_pointer, orig_fdt_blob, fdt_size);

    // Allocate resources and modify addresses in the device tree for each device.
    for (int dev_index=0; dev_index < device_count; dev_index++) {
        ret = allocate_dev_resource_and_fdt_fixup(device_paths[dev_index]);
        if (0 != ret) {
            free(uboot_fdt_pointer);
            uboot_fdt_pointer = NULL;
            shutdown_timer();
            return -1;
        }
    }

    // Start the U-Boot wrapper. Provide it a pointer to the FDT blob.
    ret = initialise_uboot_wrapper(uboot_fdt_pointer);
    if (0 != ret) {
        free(uboot_fdt_pointer);
        uboot_fdt_pointer = NULL;
        shutdown_timer();
        return ret;
    }

    // All done.
    return 0;
}

void shutdown_uboot_drivers(void) {
    if (uboot_fdt_pointer != NULL) {
        free(uboot_fdt_pointer);
        uboot_fdt_pointer = NULL;
        shutdown_timer();
    }

    shutdown_uboot_wrapper();

    sel4_io_map_shutdown();

    sel4_dma_shutdown();
}