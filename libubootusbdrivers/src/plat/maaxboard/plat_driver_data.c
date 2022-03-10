/* SPDX-License-Identifier: GPL-2.0+ */

#include <uboot_helper.h>

#include <dm/device.h>
#include <dm/uclass.h>
#include <dm/platdata.h>
#include <usb.h>

struct driver_data_t driver_data;

// TBD - believe this is not used.
struct udevice _u_boot_udevice__root; // Not defined...

void initialise_driver_data(void) {
    driver_data.driver_array[0] = _u_boot_driver__root_driver;
    driver_data.driver_array[1] = _u_boot_driver__simple_bus;
    driver_data.driver_array[2] = _u_boot_driver__dwc3_generic_wrapper;
    driver_data.driver_array[3] = _u_boot_driver__dwc3_generic_host;
    driver_data.driver_array[4] = _u_boot_driver__usb_generic_hub;

    driver_data.uclass_driver_array[0] = _u_boot_uclass_driver__nop;
    driver_data.uclass_driver_array[1] = _u_boot_uclass_driver__root;
    driver_data.uclass_driver_array[2] = _u_boot_uclass_driver__simple_bus;
    driver_data.uclass_driver_array[3] = _u_boot_uclass_driver__usb;
    driver_data.uclass_driver_array[4] = _u_boot_uclass_driver__usb_hub;
}
