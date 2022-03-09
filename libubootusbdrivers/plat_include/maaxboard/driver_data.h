/* SPDX-License-Identifier: GPL-2.0+ */

#ifndef	__DRIVER_DATA_H
#define __DRIVER_DATA_H

#include <dm/device.h>
#include <dm/uclass.h>
#include <dm/platdata.h>
#include <usb.h>

struct driver_data_t {
    struct driver driver_list[1];
	struct uclass_driver uclass_driver_list[2];
};

extern struct driver_data_t driver_data;

extern struct driver _u_boot_list_2_driver_1;
extern struct driver _u_boot_list_2_driver_3;

extern struct uclass_driver _u_boot_list_2_uclass_driver_1;
extern struct uclass_driver _u_boot_list_2_uclass_driver_3;

extern struct driver_info _u_boot_list_2_driver_info_1;
extern struct driver_info _u_boot_list_2_driver_info_3;

extern struct udevice _u_boot_list_2_udevice_1;
extern struct udevice _u_boot_list_2_udevice_3;

extern struct usb_driver_entry _u_boot_list_2_usb_driver_entry_1;
extern struct usb_driver_entry _u_boot_list_2_usb_driver_entry_3;

extern struct driver _u_boot_list_2_driver_2_root_driver;
extern struct uclass_driver _u_boot_list_2_uclass_driver_2_root;
extern struct uclass_driver _u_boot_list_2_uclass_driver_2_usb;
extern struct udevice _u_boot_list_2_udevice_2_root;

void initialise_driver_data(void);

#endif /* __DRIVER_DATA_H */