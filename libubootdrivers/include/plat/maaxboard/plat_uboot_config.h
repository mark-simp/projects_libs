/*
 * This file defines the platform specific U-Boot configuration macros
 */

/*
 * For the AvNet MaaXBoard we are using the following drivers which need
 * specific configuration:
 *
 * - USB: The MaaXBoard has an XCHI (i.e. USB 3.0) host controller. We
 *        need to provide configuration macros specific to XCHI and
 *        provide macros for the USB device classes (mass storeage and
 *        keyboard) we wish to support.
 */

/* Enable the USB subsystem */
#define CONFIG_DM_USB                   1
#define CONFIG_USB_HOST                 1 /* Allow the USB devices to be used as a host */
#define CONFIG_USB_STORAGE              1 /* Allow USB mass storeage support */
#define CONFIG_USB_KEYBOARD             1 /* Allow USB keyboard support */
#define CONFIG_SYS_USB_EVENT_POLL       1 /* Select USB keyboard polling method */

/* Enable the XHCI USB specific extensions */
#define CONFIG_USB_XHCI_HCD             1 /* State we are using an XHCI host controller */
#define CONFIG_USB_XHCI_DWC3			1 /* Enable XHCI support in the DWC3 driver */

/* Enable the MMC subsystem */
#define CONFIG_MMC                      1
#define CONFIG_DM_MMC                   1
#define CONFIG_FSL_ESDHC_IMX            1
#define CONFIG_FSL_USDHC                1
#define CONFIG_MMC_UHS_SUPPORT          1
#define CONFIG_MMC_HS200_SUPPORT        1
#define CONFIG_MMC_HS400_SUPPORT        1
#define CONFIG_MMC_VERBOSE              1
#define CONFIG_MMC_IO_VOLTAGE           1
#define CONFIG_MMC_QUIRKS               1
#define CONFIG_MMC_WRITE                1
#define CONFIG_MMC_HW_PARTITIONING      1