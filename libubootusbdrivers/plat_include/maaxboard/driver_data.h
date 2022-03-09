/* SPDX-License-Identifier: GPL-2.0+ */

#ifndef	__DRIVER_DATA_H
#define __DRIVER_DATA_H

#include <dm/device.h>
#include <dm/uclass.h>
#include <dm/platdata.h>
#include <usb.h>

struct driver_data_t {
    struct driver driver_array[1];
	struct uclass_driver uclass_driver_array[3];
    struct usb_driver_entry usb_driver_entry_array[0];
    struct driver_info driver_info_array[0];
    struct udevice udevice_array[0];
};

extern struct driver_data_t driver_data;

extern struct driver _u_boot_list_2_driver_2_root_driver;
extern struct uclass_driver _u_boot_list_2_uclass_driver_2_root;
extern struct uclass_driver _u_boot_list_2_uclass_driver_2_usb;
extern struct uclass_driver _u_boot_list_2_uclass_driver_2_usb_hub;
extern struct udevice _u_boot_list_2_udevice_2_root; // Not defined...

void initialise_driver_data(void);

#endif /* __DRIVER_DATA_H */