/* SPDX-License-Identifier: GPL-2.0+ */

#include <uboot_helper.h>

#include <dm/device.h>
#include <dm/uclass.h>
#include <dm/platdata.h>
#include <usb.h>

struct driver_data_t driver_data;

struct udevice _u_boot_list_2_udevice_2_root; // Not defined...

void initialise_driver_data(void) {
    driver_data.driver_array[0] = _u_boot_list_2_driver_2_root_driver;

    driver_data.uclass_driver_array[0] = _u_boot_list_2_uclass_driver_2_root;
    driver_data.uclass_driver_array[1] = _u_boot_list_2_uclass_driver_2_usb;
    driver_data.uclass_driver_array[2] = _u_boot_list_2_uclass_driver_2_usb_hub;
}
