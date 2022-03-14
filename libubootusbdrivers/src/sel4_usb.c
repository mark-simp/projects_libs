#include <sel4platsupport/io.h>
#include <plat_resources.h>
#include <libfdt.h>
#include <uboot_wrapper.h>

#define MAP_DEVICE(o, p, s) ps_io_map(&o->io_mapper, p, s, 0, PS_MEM_NORMAL)
#define GET_RESOURCE(ops, id) MAP_DEVICE(ops, id##_PADDR, id##_SIZE)

void* uboot_fdt_pointer = NULL;

int allocate_dev_resource_and_fdt_fixup(ps_io_ops_t *io_ops, const char* path) {

    // Lookup the address and size information form the memory mapped device.
    int node_offset = fdt_path_offset(uboot_fdt_pointer, path);
    if (node_offset < 0) {
        ZF_LOGE("Unable to find path '%s' in device tree.\n", path);
        return node_offset;
    }

    int parent_node_offset = fdt_parent_offset(uboot_fdt_pointer, node_offset);
    if (parent_node_offset < 0) {
        ZF_LOGE("Unable to find parent of path '%s' in device tree.\n", path);
        return node_offset;
    }

    const struct fdt_property* reg_property = malloc(sizeof(struct fdt_property));

    int prop_len;
    reg_property = fdt_get_property(uboot_fdt_pointer, node_offset, "reg", &prop_len);
    if (reg_property == NULL) {
        ZF_LOGE("Path '%s' had no 'reg' property in device tree.\n", path);
        return prop_len;
    }

    int addr_cells = fdt_address_cells(uboot_fdt_pointer, parent_node_offset);
    if (addr_cells < 1 || addr_cells > 2) {
        ZF_LOGE("Path '%s' has address other than 32 or 64 bits.", path);
        return -1;
    }

    int size_cells = fdt_size_cells(uboot_fdt_pointer, parent_node_offset);
    if (size_cells < 1 || size_cells > 2) {
        ZF_LOGE("Path '%s' has size other than 32 or 64 bits.", path);
        return -1;
    }

    uintptr_t physical_address;
    if (addr_cells == 2)
        physical_address = (uintptr_t) fdt64_to_cpu(*(fdt64_t *) &reg_property->data[0]);
    else
        physical_address = (uintptr_t) fdt32_to_cpu(*(fdt32_t *) &reg_property->data[0]);

    size_t device_size;
    if (size_cells == 2)
        device_size = (size_t) fdt64_to_cpu(*(fdt64_t *) &reg_property->data[4 * addr_cells]);
    else
        device_size = (size_t) fdt32_to_cpu(*(fdt32_t *) &reg_property->data[4 * addr_cells]);

    if ((addr_cells + size_cells) * 4 != prop_len) {
        ZF_LOGE("Path '%s' has 'reg' property consisting of more than a single address, size pair. Unhandled.");
        return -1;
    }

    ZF_LOGD("Mapping device for path '%s' with address %p and size %x.",
            path, physical_address, device_size);

    // We now have a known and verified address and size for hte device.
    // Map it into our virtual address space.
    void* virtual_address = ps_io_map(&io_ops->io_mapper, physical_address, device_size, 0, PS_MEM_NORMAL);
    if (NULL == virtual_address) {
        ZF_LOGE("Unable to map virtual address for path '%s'.", path);
        return -1;
    }

    // Update the FDT to point to the virtual rather than physical address.
    uint8_t tmp[prop_len];
    char *p = tmp;

    if (addr_cells == 2)
        *(fdt64_t *)p = cpu_to_fdt64((uintptr_t) virtual_address);
    else
        *(fdt32_t *)p = cpu_to_fdt32((uintptr_t) virtual_address);
    p += 4 * addr_cells;

    if (size_cells == 2)
        *(fdt64_t *)p = cpu_to_fdt64(fdt64_to_cpu(device_size));
    else
        *(fdt32_t *)p = cpu_to_fdt32(fdt32_to_cpu(device_size));

    int ret = fdt_setprop(uboot_fdt_pointer, node_offset, "reg", tmp, prop_len);
    if (0 != ret) {
        ZF_LOGE("Unable to update device for path '%s' with device virtual address.", path);
        return -1;
    }

    return 0;
}

int sel4_usb_init(ps_io_ops_t *io_ops, const char **device_paths, uint32_t device_count)
{
    void* orig_fdt_blob = io_ops->io_fdt.get_fn(io_ops->io_fdt.cookie);

    // Create a copy of the FDT for U-Boot to use.
    int fdt_size = fdt_totalsize(orig_fdt_blob);
    uboot_fdt_pointer = malloc(fdt_size);
    memcpy(uboot_fdt_pointer, orig_fdt_blob, fdt_size);

    // Allocate resoucres and modify addresses in the device tree for each device.
    int ret;
    for (int dev_index=0; dev_index < device_count; dev_index++) {
        ret = allocate_dev_resource_and_fdt_fixup(io_ops, device_paths[dev_index]);
        if (0 != ret) {
            free(uboot_fdt_pointer);
            return -1;
        }
    }

    // Start the U-Boot device model and provide it a pointer to the FDT blob.
    ret = init_uboot(uboot_fdt_pointer);
    if (ret) {
        free(uboot_fdt_pointer);
        return ret;
    }

    return 0;
}