#include <uboot_helper.h>
#include <linux/types.h>
#include <usb.h>

int sel4_usb_init(void)
{
    return usb_init();
}