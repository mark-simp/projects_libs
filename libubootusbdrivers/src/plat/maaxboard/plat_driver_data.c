/* SPDX-License-Identifier: GPL-2.0+ */

#include <uboot_helper.h>

#include <dm/device.h>
#include <dm/uclass.h>
#include <dm/platdata.h>
#include <usb.h>

struct driver_data_t driver_data;

// Note: The following is only
struct udevice _u_boot_udevice__root; // Not defined...

void initialise_driver_data(void) {
    driver_data.driver_array[0] = _u_boot_driver__root_driver;

    driver_data.uclass_driver_array[0] = _u_boot_uclass_driver__root;
    driver_data.uclass_driver_array[1] = _u_boot_uclass_driver__usb;
    driver_data.uclass_driver_array[2] = _u_boot_uclass_driver__usb_hub;
}
