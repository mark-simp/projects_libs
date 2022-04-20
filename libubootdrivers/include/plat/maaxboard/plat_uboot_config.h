/*
 * This file defines the platform specific U-Boot configuration macros
 */

/* Enable pin control / mux support (required by other drivers) */
#define CONFIG_PINCTRL                  1
#define CONFIG_PINCTRL_FULL             1
#define CONFIG_PINCTRL_GENERIC          1
#define CONFIG_PINMUX                   1
#define CONFIG_PINCONF_RECURSIVE        1

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

/* Enable FAT partition and filesystem support */
#define CONFIG_FS_FAT                   1
#define CONFIG_FAT_WRITE                1
#define CONFIG_FS_FAT_MAX_CLUSTSIZE     65536
#define CONFIG_DOS_PARTITION            1

/* Enable ethernet */
#define CONFIG_DM_ETH                   1
#define CONFIG_PHYLIB                   1
// CONFIG_PHY_ATHEROS=y
// CONFIG_NETDEVICES=y
// CONFIG_PHY_GIGE=y
// CONFIG_FEC_MXC=y
#define CONFIG_MII                      1