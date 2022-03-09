/* SPDX-License-Identifier: GPL-2.0+ */

#include <uboot_helper.h>

#include <dm/device.h>
#include <dm/uclass.h>
#include <dm/platdata.h>
#include <usb.h>

struct driver_data_t driver_data;

struct driver _u_boot_list_2_driver_1;
struct driver _u_boot_list_2_driver_3;

struct uclass_driver _u_boot_list_2_uclass_driver_1;
struct uclass_driver _u_boot_list_2_uclass_driver_3;

struct driver_info _u_boot_list_2_driver_info_1; // Not defined...
struct driver_info _u_boot_list_2_driver_info_3; // Not defined...

struct udevice _u_boot_list_2_udevice_1; // Not defined...
struct udevice _u_boot_list_2_udevice_3; // Not defined...

struct usb_driver_entry _u_boot_list_2_usb_driver_entry_1; // Not defined...
struct usb_driver_entry _u_boot_list_2_usb_driver_entry_3; // Not defined...

struct udevice _u_boot_list_2_udevice_2_root; // Not defined...

void initialise_driver_data(void) {
    driver_data.driver_list[0] = _u_boot_list_2_driver_2_root_driver;
    _u_boot_list_2_driver_1 = driver_data.driver_list[0];
    _u_boot_list_2_driver_3 = driver_data.driver_list[0];

    driver_data.uclass_driver_list[0] = _u_boot_list_2_uclass_driver_2_root;
    driver_data.uclass_driver_list[1] = _u_boot_list_2_uclass_driver_2_usb;
    _u_boot_list_2_uclass_driver_1 = driver_data.uclass_driver_list[0];
    _u_boot_list_2_uclass_driver_3 = driver_data.uclass_driver_list[1];
}
