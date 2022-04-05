/* SPDX-License-Identifier: GPL-2.0+ */

#ifndef	__DRIVER_DATA_H
#define __DRIVER_DATA_H

#include <dm/device.h>
#include <dm/uclass.h>
#include <dm/platdata.h>
#include <usb.h>
#include <command.h>
#include <env.h>
#include <env_internal.h>

#include <plat_driver_data.h>

struct driver_data_t {
    /* The array of drivers compiled in */
    struct driver driver_array[DEFINE_UBOOT_DRIVER_COUNT];
    /* The array of driver classes compiled in */
	struct uclass_driver uclass_driver_array[DEFINE_UBOOT_UCLASS_DRIVER_COUNT];
    /* The array of USB driver entries compiled in (e.g. mass storeage, keyboard, etc) */
    struct usb_driver_entry usb_driver_entry_array[DEFINE_UBOOT_USB_DRIVER_ENTRY_COUNT];
    /* The array of commands compiled in */
    struct cmd_tbl cmd_array[DEFINE_UBOOT_CMD_COUNT];
    /* Entries to be used for drivers for pre-relocation environment data,
     * e.g. environment data stored in EEPROM. We do not use these and
     * instead rely upon the default environment data (see env_default.h)
     * and environment variables explicitly set up by the user of the library.
     * Callbacks triggered by environment variable changes are also not
     * supported.
     */
    struct env_driver env_driver_array[0];
    struct env_clbk_tbl env_clbk_array[0];
    /*
     * Entries used for 'static' drivers, i.e. drivers to be
     * statically bound to devices. These are explicitly empty / unused;
     * drivers are to be bound to devices at run-time based upon the
     * contents of the device tree and the compatibility of the driver.
     */
    struct driver_info driver_info_array[0];
    struct udevice udevice_array[0];
};

extern struct driver_data_t driver_data;

void initialise_driver_data(void);

#endif /* __DRIVER_DATA_H */