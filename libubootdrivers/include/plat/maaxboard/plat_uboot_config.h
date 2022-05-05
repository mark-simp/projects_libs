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
#define CONFIG_ETH                      1
#define CONFIG_NET                      1
#define CONFIG_DM_ETH                   1
#define CONFIG_PHYLIB                   1
#define CONFIG_PHY_ATHEROS              1
#define CONFIG_PHY_GIGE                 1
#define CONFIG_FEC_MXC                  1
#define CONFIG_MII                      1
#define CONFIG_TFTP_BLOCKSIZE           1468
#define CONFIG_TFTP_WINDOWSIZE          1
#define CONFIG_NET_RANDOM_ETHADDR       1
#define CONFIG_CMD_NET                  1
#define CONFIG_CMD_DNS                  1
#define CONFIG_CMD_PING                 1

/* Enable GPIO */
#define CONFIG_DM_GPIO                  1
#define CONFIG_GPIO_EXTRA_HEADER        1
#define CONFIG_CMD_GPIO_READ            1

/* Enable I2C */
#define CONFIG_CMD_I2C                  1
#define CONFIG_DM_I2C                   1
#define CONFIG_SYS_I2C_MXC              1
#define CONFIG_SYS_I2C_MXC_I2C1         1
#define CONFIG_SYS_I2C_MXC_I2C2         1
#define CONFIG_SYS_I2C_MXC_I2C3         1
#define CONFIG_SYS_I2C_MXC_I2C4         1

#define CONFIG_SYS_MXC_I2C1_SPEED       100000
#define CONFIG_SYS_MXC_I2C1_SLAVE       0
#define CONFIG_SYS_MXC_I2C2_SPEED       100000
#define CONFIG_SYS_MXC_I2C2_SLAVE       0
#define CONFIG_SYS_MXC_I2C3_SPEED       100000
#define CONFIG_SYS_MXC_I2C3_SLAVE       0
#define CONFIG_SYS_MXC_I2C4_SPEED       100000
#define CONFIG_SYS_MXC_I2C4_SLAVE       0

/* Enable SPI */
#define CONFIG_DM_SPI_FLASH             1
#define CONFIG_DM_SPI                   1
#define CONFIG_SPI                      1
#define CONFIG_SPI_FLASH                1
#define CONFIG_SPI_FLASH_BAR            1
#define CONFIG_SPI_FLASH_STMICRO        1
//#define CONFIG_SYS_SPI_MXC_WAIT         5000
#define CONFIG_DEFAULT_SPI_MODE         0
#define CONFIG_DEFAULT_SPI_BUS          0