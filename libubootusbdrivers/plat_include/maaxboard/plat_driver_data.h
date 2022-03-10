// Define the number of different driver elements to be used on this platform.
#define DEFINE_UBOOT_UCLASS_DRIVER_COUNT        4
#define DEFINE_UBOOT_DRIVER_COUNT               5
#define DEFINE_UBOOT_USB_DRIVER_ENTRY_COUNT     1
#define DEFINE_UBOOT_DRIVER_INFO_COUNT          0
#define DEFINE_UBOOT_UDEVICE_COUNT              0

// Define the uclass drivers to be used on this platform.
extern struct uclass_driver _u_boot_uclass_driver__root;
extern struct uclass_driver _u_boot_uclass_driver__simple_bus;
extern struct uclass_driver _u_boot_uclass_driver__usb;
extern struct uclass_driver _u_boot_uclass_driver__usb_hub;

// Define the drivers to be used on this platform.
extern struct driver _u_boot_driver__root_driver;
extern struct driver _u_boot_driver__simple_bus;
extern struct driver _u_boot_driver__dwc3_generic_wrapper;
extern struct driver _u_boot_driver__dwc3_generic_host;
extern struct driver _u_boot_driver__usb_generic_hub;

// TBD
extern struct usb_driver_entry _u_boot_usb_driver_entry__usb_generic_hub;