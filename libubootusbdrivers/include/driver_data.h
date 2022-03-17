/* SPDX-License-Identifier: GPL-2.0+ */

#ifndef	__DRIVER_DATA_H
#define __DRIVER_DATA_H

#include <dm/device.h>
#include <dm/uclass.h>
#include <dm/platdata.h>
#include <usb.h>

#include <plat_driver_data.h>

struct driver_data_t {
    /* The array of drivers compiled in */
    struct driver driver_array[DEFINE_UBOOT_DRIVER_COUNT];
    /* The array of driver classes compiled in */
	struct uclass_driver uclass_driver_array[DEFINE_UBOOT_UCLASS_DRIVER_COUNT];
    /*
     * The remaining entries are used for 'static' drivers, i.e. drivers to
     * be statically bound to devices. These are normally empty / unused;
     * drivers are normally bound to devices at run-time based upon the
     * contents of the device tree and the compatibility of the driver.
     */
    struct usb_driver_entry usb_driver_entry_array[DEFINE_UBOOT_USB_DRIVER_ENTRY_COUNT];
    struct driver_info driver_info_array[DEFINE_UBOOT_DRIVER_INFO_COUNT];
    struct udevice udevice_array[DEFINE_UBOOT_UDEVICE_COUNT];
};

extern struct driver_data_t driver_data;

void initialise_driver_data(void);

#endif /* __DRIVER_DATA_H */