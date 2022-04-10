/*
 * This file defines which drivers, driver classes, driver entries and commands
 * are to be included in the compiled library for this platform.
 *
 * This allows only the drivers compatible with the targeted platform to be
 * included with all non-compatible drivers excluded.
 *
 * It should be noted that some of these are fundamental to allowing the U-Boot
 * driver model to function (e.g. the nop, root and simple bus drivers).
 */

/* Define the number of different driver elements to be used on this platform */
#define DEFINE_UBOOT_UCLASS_DRIVER_COUNT        11
#define DEFINE_UBOOT_DRIVER_COUNT               12
#define DEFINE_UBOOT_USB_DRIVER_ENTRY_COUNT     3
#define DEFINE_UBOOT_PART_DRIVER_COUNT          1
#define DEFINE_UBOOT_CMD_COUNT                  10

/* Define the uclass drivers to be used on this platform */
extern struct uclass_driver _u_boot_uclass_driver__nop;
extern struct uclass_driver _u_boot_uclass_driver__root;
extern struct uclass_driver _u_boot_uclass_driver__simple_bus;
extern struct uclass_driver _u_boot_uclass_driver__usb;
extern struct uclass_driver _u_boot_uclass_driver__usb_hub;
extern struct uclass_driver _u_boot_uclass_driver__usb_dev_generic;
extern struct uclass_driver _u_boot_uclass_driver__phy;
extern struct uclass_driver _u_boot_uclass_driver__usb_mass_storage;
extern struct uclass_driver _u_boot_uclass_driver__blk;
extern struct uclass_driver _u_boot_uclass_driver__keyboard;
extern struct uclass_driver _u_boot_uclass_driver__mmc;

/* Define the drivers to be used on this platform */
extern struct driver _u_boot_driver__root_driver;
extern struct driver _u_boot_driver__simple_bus;
extern struct driver _u_boot_driver__usb_dev_generic_drv;
extern struct driver _u_boot_driver__usb_generic_hub;
extern struct driver _u_boot_driver__dwc3_generic_wrapper;
extern struct driver _u_boot_driver__dwc3_generic_host;
extern struct driver _u_boot_driver__nxp_imx8mq_usb_phy;
extern struct driver _u_boot_driver__usb_mass_storage;
extern struct driver _u_boot_driver__usb_storage_blk;
extern struct driver _u_boot_driver__usb_kbd;
extern struct driver _u_boot_driver__mmc_blk;
extern struct driver _u_boot_driver__fsl_esdhc;

/* Define the driver entries to be used on this platform */
extern struct usb_driver_entry _u_boot_usb_driver_entry__usb_generic_hub;
extern struct usb_driver_entry _u_boot_usb_driver_entry__usb_mass_storage;
extern struct usb_driver_entry _u_boot_usb_driver_entry__usb_kbd;

/* Define the disk partition types to be used */
extern struct part_driver _u_boot_part_driver__dos;

/* Define the u-boot commands to be used on this platform */
extern struct cmd_tbl _u_boot_cmd__dm;
extern struct cmd_tbl _u_boot_cmd__usb;
extern struct cmd_tbl _u_boot_cmd__env;
extern struct cmd_tbl _u_boot_cmd__setenv;
extern struct cmd_tbl _u_boot_cmd__mmc;
extern struct cmd_tbl _u_boot_cmd__fatsize;
extern struct cmd_tbl _u_boot_cmd__fatload;
extern struct cmd_tbl _u_boot_cmd__fatls;
extern struct cmd_tbl _u_boot_cmd__fatinfo;
extern struct cmd_tbl _u_boot_cmd__part;