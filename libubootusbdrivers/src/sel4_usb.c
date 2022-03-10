#include <sel4platsupport/io.h>
#include <plat_resources.h>
#include <uboot_wrapper.h>

#define MAP_DEVICE(o, p, s) ps_io_map(&o->io_mapper, p, s, 0, PS_MEM_NORMAL)
#define GET_RESOURCE(ops, id) MAP_DEVICE(ops, id##_PADDR, id##_SIZE)

int sel4_usb_init(ps_io_ops_t *io_ops)
{
    // Allocate resoucres.
    void *usb_regs_1 = GET_RESOURCE(io_ops, USB_HOST0_XHCI);
    void *usb_regs_2 = GET_RESOURCE(io_ops, USB_HOST1_XHCI);

    if (usb_regs_1 == NULL || usb_regs_2 == NULL)
        return -1;

    // Start the U-Boot device model and provide it a pointer to the FDT blob.
    int ret = init_uboot(io_ops->io_fdt.get_fn(io_ops->io_fdt.cookie));
    if (ret)
        return ret;

    return 0;
}