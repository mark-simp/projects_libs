#
# SPDX-License-Identifier: BSD-2-Clause
#

cmake_minimum_required(VERSION 3.7.2)

project(libubootdrivers C)

# Set the logging level for the library (e.g. ZF_LOG_INFO, ZF_LOG_VERBOSE, etc)
set(LIB_UBOOT_LOGGING_LEVEL "ZF_LOG_INFO")
add_definitions("-DZF_LOG_LEVEL=${LIB_UBOOT_LOGGING_LEVEL}")

config_option(LibUBOOT LIB_UBOOTB "Build USB interface library" DEFAULT ON)

add_config_library(ubootdrivers "${configure_string}")
if(LibUBOOT)

    add_library(ubootdrivers STATIC EXCLUDE_FROM_ALL "")

    ##################################
    # Architecture specific settings #
    ##################################

    if(KernelArchARM)
        # Set architecture as ARM.
        add_definitions("-DCONFIG_ARM")

        # Tell U-Boot code whether this is ARM32 or ARM64. Note that U-Boot
        # code defaults to 32Bit, we only need to explicitly provide definitions
        # to switch to 64Bit addressing.
        if(Kernel64)
            add_definitions("-DCONFIG_ARM64")
            add_definitions("-DCONFIG_DMA_ADDR_T_64BIT")
            add_definitions("-DCONFIG_PHYS_64BIT")
        endif()

        # Tell U-Boot code which generation of ARM we are using.
        if(KernelArchArmV8a)
            add_definitions("-D__LINUX_ARM_ARCH__=8")
        elseif(KernelArchArmV7a OR KernelArchArmV7ve)
            add_definitions("-D__LINUX_ARM_ARCH__=7")
        endif()

        # Tell U-Boot code the processor's cacheline size.
        math(EXPR CACHELINE_SIZE "1 << ${KernelArmCacheLineSizeBits}")
        add_definitions("-DCONFIG_SYS_CACHELINE_SIZE=${CACHELINE_SIZE}")

        # Architecture specific include files
        target_include_directories(ubootdrivers PRIVATE include/stub/arch/arm)
        target_include_directories(ubootdrivers PRIVATE include/uboot/arch/arm)
        target_include_directories(ubootdrivers PRIVATE include/uboot/arch/proc-armv)
        if(KernelArchArmV8a)
            target_include_directories(ubootdrivers PRIVATE include/uboot/arch/armv8)
        endif()
    endif()

    ##############################
    # Platform specific settings #
    ##############################

    # By default set all of the drivers to use to null / dummy settings.
    set(timer_driver "dummy")
    set(clock_driver "none")
    set(iomux_driver "none")
    set(gpio_driver "none")
    set(eth_driver "none")
    set(eth_phy_driver "none")
    set(usb_driver "none")
    set(usb_phy_driver "none")
    set(mmc_driver "none")
    set(i2c_driver "none")
    set(spi_driver "none")
    set(led_driver "none")
    set(disk_support "false")
    set(usb_storage_support "false")
    set(usb_keyboard_support "false")

    # Set up the applicable drivers and platform dependent configuration.
    if(KernelPlatImx8mq)
        # Platform specific configuration
        add_definitions("-DCONFIG_ARCH_IMX8M=1")
        add_definitions("-DCONFIG_IMX8M=1")
        add_definitions("-DCONFIG_IMX8MQ=1")

        # Define the drivers used for this platform
        set(timer_driver "imx8mq")
        set(clock_driver "imx8mq")
        set(iomux_driver "imx8mq-pinctrl")
        set(gpio_driver "gpio_mxc")
        set(eth_driver "fecmxc")
        set(usb_driver "dwc3_xhci")
        set(usb_phy_driver "nxp_imx8mq_usb_phy")
        set(mmc_driver "fsl_esdhc")
        set(led_driver "gpio_led")
        set(disk_support "true")
        set(usb_storage_support "true")
        set(usb_keyboard_support "true")

        # Set the Ethernet PHY based upon the board
        if(KernelPlatformMaaxboard OR KernelPlatformImx8mq-evk)
            set(eth_phy_driver "atheros")
        else()
            message(FATAL_ERROR "Unsupported iMX8MQ board. Aborting.")
        endif()

        # Platform specific include files
        target_include_directories(ubootdrivers PRIVATE include/uboot/arch/imx8m)

    elseif(KernelPlatImx6)
        # TBC

    else()
        message(FATAL_ERROR "Unsupported platform. Aborting.")
    endif()

    ##############################
    # Settings for timer drivers #
    ##############################

    if(timer_driver MATCHES "dummy")
        list(APPEND uboot_deps src/timer/timer_dummy.c)
    elseif(timer_driver MATCHES "imx8mq")
        list(APPEND uboot_deps src/timer/timer_imx8mq.c)
    else()
        message(FATAL_ERROR "Unrecognised timer driver. Aborting.")
    endif()

    ##############################
    # Settings for clock drivers #
    ##############################

    if(clock_driver MATCHES "none")
        # Nothing to do
    elseif(clock_driver MATCHES "imx8mq")
        list(APPEND uboot_deps src/uboot/arch/arm/mach-imx/imx8m/clock_imx8mq.c)
        list(APPEND uboot_deps src/uboot/arch/arm/mach-imx/imx8m/clock_slice.c)
    else()
        message(FATAL_ERROR "Unrecognised clock driver. Aborting.")
    endif()

    ##############################
    # Settings for IOMUX drivers #
    ##############################

    if(iomux_driver MATCHES "none")
        # Nothing to do
    else()
        # Enable IOMUX support
        add_definitions("-DCONFIG_PINCTRL=1")
        add_definitions("-DCONFIG_PINCTRL_FULL=1")
        add_definitions("-DCONFIG_PINCTRL_GENERIC=1")
        add_definitions("-DCONFIG_PINMUX=1")
        add_definitions("-DCONFIG_PINCONF_RECURSIVE=1")
        # Generic IOMUX source files
        list(APPEND uboot_deps src/uboot/src/drivers/pinctrl/pinctrl-generic.c)
        list(APPEND uboot_deps src/uboot/src/drivers/pinctrl/pinctrl-uclass.c)

        # Driver specific settings / files
        if(iomux_driver MATCHES "imx8mq-pinctrl")
            list(APPEND uboot_deps src/uboot/src/drivers/pinctrl/nxp/pinctrl-imx.c)
            list(APPEND uboot_deps src/uboot/src/drivers/pinctrl/nxp/pinctrl-imx8m.c)
        else()
            message(FATAL_ERROR "Unrecognised IOMUX driver. Aborting.")
        endif()
    endif()

    #############################
    # Settings for GPIO drivers #
    #############################

    if(gpio_driver MATCHES "none")
        # Nothing to do
    else()
        # Enable GPIO support
        add_definitions("-DCONFIG_DM_GPIO=1")
        add_definitions("-DCONFIG_GPIO_EXTRA_HEADER=1")
        add_definitions("-DCONFIG_CMD_GPIO_READ=1")
        # Generic GPIO source files
        list(APPEND uboot_deps src/uboot/src/cmd/gpio.c)
        list(APPEND uboot_deps src/uboot/src/drivers/gpio/gpio_uclass.c)

        # Driver specific settings / files
        if(gpio_driver MATCHES "gpio_mxc")
            list(APPEND uboot_deps src/uboot/src/drivers/gpio/mxc_gpio.c)
        else()
            message(FATAL_ERROR "Unrecognised GPIO driver. Aborting.")
        endif()
    endif()

    #################################
    # Settings for Ethernet drivers #
    #################################

    if(eth_driver MATCHES "none")
        # Nothing to do
    else()
        # Enable Ethernet support
        add_definitions("-DCONFIG_ETH=1")
        add_definitions("-DCONFIG_NET=1")
        add_definitions("-DCONFIG_DM_ETH=1")
        add_definitions("-DCONFIG_TFTP_BLOCKSIZE=1468")
        add_definitions("-DCONFIG_TFTP_WINDOWSIZE=1")
        add_definitions("-DCONFIG_DM_ETH=1")
        add_definitions("-DCONFIG_CMD_NET=1")
        add_definitions("-DCONFIG_CMD_DNS=1")
        add_definitions("-DCONFIG_CMD_PING=1")
        # Generic Ethernet source files
        list(APPEND uboot_deps src/uboot/src/cmd/net.c)
        list(APPEND uboot_deps src/uboot/src/drivers/gpio/gpio_uclass.c)
        list(APPEND uboot_deps src/uboot/src/net/arp.c)
        list(APPEND uboot_deps src/uboot/src/net/dns.c)
        list(APPEND uboot_deps src/uboot/src/net/eth_common.c)
        list(APPEND uboot_deps src/uboot/src/net/eth-uclass.c)
        list(APPEND uboot_deps src/uboot/src/net/net.c)
        list(APPEND uboot_deps src/uboot/src/net/pcap.c)
        list(APPEND uboot_deps src/uboot/src/net/ping.c)
        list(APPEND uboot_deps src/uboot/src/net/sntp.c)
        list(APPEND uboot_deps src/uboot/src/net/tftp.c)
        list(APPEND uboot_deps src/uboot/src/net/udp.c)

        # Driver specific settings / files
        if(eth_driver MATCHES "fecmxc")
            add_definitions("-DCONFIG_PHYLIB=1")
            add_definitions("-DCONFIG_FEC_MXC=1")
            add_definitions("-DCONFIG_MII=1")
            add_definitions("-DCONFIG_NET_RANDOM_ETHADDR=1")

            list(APPEND uboot_deps src/uboot/arch/arm/mach-imx/mac.c)
            list(APPEND uboot_deps src/uboot/src/common/miiphyutil.c)
            list(APPEND uboot_deps src/uboot/src/drivers/net/fec_mxc.c)
        else()
            message(FATAL_ERROR "Unrecognised Ethernet driver. Aborting.")
        endif()
    endif()

    #####################################
    # Settings for Ethernet PHY drivers #
    #####################################

    if(eth_phy_driver MATCHES "none")
        # Nothing to do
    elseif (eth_driver MATCHES "none")
        message(FATAL_ERROR "Cannot enable Ethernet PHY driver without Ethernet driver. Aborting.")
    elseif(eth_phy_driver MATCHES "atheros")
        add_definitions("-DCONFIG_PHY_ATHEROS=1")
        add_definitions("-DCONFIG_PHY_GIGE=1")
        list(APPEND uboot_deps src/uboot/src/drivers/net/phy/atheros.c)
        list(APPEND uboot_deps src/uboot/src/drivers/net/phy/phy.c)
    else()
        message(FATAL_ERROR "Unrecognised Ethernet PHY driver. Aborting.")
    endif()

    ############################
    # Settings for USB drivers #
    ############################

    if(usb_driver MATCHES "none")
        # Nothing to do
    else()
        # Enable USB support
        add_definitions("-DCONFIG_DM_USB=1")
        add_definitions("-DCONFIG_USB_HOST=1")
        add_definitions("-DCONFIG_SYS_USB_EVENT_POLL=1")

        # Generic USB source files
        list(APPEND uboot_deps src/uboot/src/cmd/usb.c)
        list(APPEND uboot_deps src/uboot/src/common/usb_hub.c)
        list(APPEND uboot_deps src/uboot/src/common/usb.c)
        list(APPEND uboot_deps src/uboot/src/drivers/usb/common/common.c)
        list(APPEND uboot_deps src/uboot/src/drivers/usb/gadget/udc-core.c)
        list(APPEND uboot_deps src/uboot/src/drivers/usb/host/usb-uclass.c)

        # Driver specific settings / files
        if(usb_driver MATCHES "dwc3_xhci")
            add_definitions("-DCONFIG_USB_XHCI_HCD=1")
            add_definitions("-DCONFIG_USB_XHCI_DWC3=1")

            list(APPEND uboot_deps src/uboot/src/drivers/usb/host/xhci-mem.c)
            list(APPEND uboot_deps src/uboot/src/drivers/usb/host/xhci-ring.c)
            list(APPEND uboot_deps src/uboot/src/drivers/usb/host/xhci.c)
            list(APPEND uboot_deps src/uboot/src/drivers/usb/dwc3/core.c)
            list(APPEND uboot_deps src/uboot/src/drivers/usb/dwc3/dwc3-generic.c)
            list(APPEND uboot_deps src/uboot/src/drivers/usb/dwc3/ep0.c)
        else()
            message(FATAL_ERROR "Unrecognised USB driver. Aborting.")
        endif()
    endif()

    ################################
    # Settings for USB PHY drivers #
    ################################

    if(usb_phy_driver MATCHES "none")
        # Nothing to do
    elseif (usb_driver MATCHES "none")
        message(FATAL_ERROR "Cannot enable USB PHY driver without USB driver. Aborting.")
    elseif(usb_phy_driver MATCHES "nxp_imx8mq_usb_phy")
        list(APPEND uboot_deps src/uboot/src/drivers/phy/phy-imx8mp-usb.c)
    else()
        message(FATAL_ERROR "Unrecognised USB PHY driver. Aborting.")
    endif()

    ############################
    # Settings for MMC drivers #
    ############################

    if(mmc_driver MATCHES "none")
        # Nothing to do
    else()
        # Enable MMC support
        add_definitions("-DCONFIG_MMC=1")
        add_definitions("-DCONFIG_DM_MMC=1")
        add_definitions("-DCONFIG_MMC_VERBOSE=1")
        add_definitions("-DCONFIG_MMC_QUIRKS=1")
        add_definitions("-DCONFIG_MMC_WRITE=1")
        add_definitions("-DCONFIG_MMC_HW_PARTITIONING=1")
        add_definitions("-DCONFIG_MMC_IO_VOLTAGE=1")
        # Generic MMC source files
        list(APPEND uboot_deps src/uboot/src/cmd/mmc.c)
        list(APPEND uboot_deps src/uboot/src/drivers/mmc/mmc_write.c)
        list(APPEND uboot_deps src/uboot/src/drivers/mmc/mmc-uclass.c)
        list(APPEND uboot_deps src/uboot/src/drivers/mmc/mmc.c)

        # Driver specific settings / files
        if(mmc_driver MATCHES "fsl_esdhc")
            # Config for platform specifc capabilities
            if(KernelPlatImx8mq)
                add_definitions("-DCONFIG_FSL_ESDHC_IMX=1")
                add_definitions("-DCONFIG_FSL_USDHC=1")
                add_definitions("-DCONFIG_MMC_UHS_SUPPORT=1")
                add_definitions("-DCONFIG_MMC_HS200_SUPPORT=1")
                add_definitions("-DCONFIG_MMC_HS400_SUPPORT=1")
            else()
                message(FATAL_ERROR "Unrecognised MMC ESDHC platform. Aborting.")
            endif()

            list(APPEND uboot_deps src/uboot/src/drivers/mmc/fsl_esdhc_imx.c)
        else()
            message(FATAL_ERROR "Unrecognised MMC driver. Aborting.")
        endif()
    endif()

    ############################
    # Settings for LED drivers #
    ############################

    if(led_driver MATCHES "none")
        # Nothing to do
    else()
        # Generic LED source files
        list(APPEND uboot_deps src/uboot/src/cmd/led.c)
        list(APPEND uboot_deps src/uboot/src/drivers/led/led-uclass.c)

        # Driver specific settings / files
        if(led_driver MATCHES "gpio_led")
            if(gpio_driver MATCHES "none")
                message(FATAL_ERROR "Cannot enable LED GPIO driver without GPIO driver. Aborting.")
            endif()
            list(APPEND uboot_deps src/uboot/src/drivers/led/led_gpio.c)
        else()
            message(FATAL_ERROR "Unrecognised LED driver. Aborting.")
        endif()
    endif()

    #############################
    # Settings for disk support #
    #############################

    if (disk_support MATCHES "false")
        # Nothing to do
    elseif(disk_support MATCHES "true")
        # Enabled partition support
        add_definitions("-DCONFIG_HAVE_BLOCK_DEVICE=1")
        add_definitions("-DCONFIG_PARTITIONS=1")
        add_definitions("-DCONFIG_PARTITION_UUIDS=1")
        # Enable FAT filesystem support
        add_definitions("-DCONFIG_FS_FAT=1")
        add_definitions("-DCONFIG_FAT_WRITE=1")
        add_definitions("-DCONFIG_FS_FAT_MAX_CLUSTSIZE=65536")
        add_definitions("-DCONFIG_DOS_PARTITION=1")
        # Add srouce files
        list(APPEND uboot_deps src/uboot/src/cmd/fat.c)
        list(APPEND uboot_deps src/uboot/src/cmd/part.c)
        list(APPEND uboot_deps src/uboot/src/disk/part_dos.c)
        list(APPEND uboot_deps src/uboot/src/disk/part.c)
        list(APPEND uboot_deps src/uboot/src/fs/fat/fat_write.c)
        list(APPEND uboot_deps src/uboot/src/fs/fs.c)
    else()
        message(FATAL_ERROR "Unrecognised disk support level. Aborting.")
    endif()

    #########################################
    # Settings for USB mass storage support #
    #########################################

    if (usb_storage_support MATCHES "false")
        # Nothing to do
    elseif(usb_storage_support MATCHES "true")
        if(usb_driver MATCHES "none")
            message(FATAL_ERROR "Cannot enable USB mass storage without USB driver. Aborting.")
        endif()
        # Enabled support
        add_definitions("-DCONFIG_USB_STORAGE=1")
        # Add srouce files
        list(APPEND uboot_deps src/uboot/src/common/usb_storage.c)
    else()
        message(FATAL_ERROR "Unrecognised USB mass storage support level. Aborting.")
    endif()

    #####################################
    # Settings for USB keyboard support #
    #####################################

    if (usb_keyboard_support MATCHES "false")
        # Nothing to do
    elseif(usb_keyboard_support MATCHES "true")
        if(usb_driver MATCHES "none")
            message(FATAL_ERROR "Cannot enable USB keyboard without USB driver. Aborting.")
        endif()
        # Enabled support
        add_definitions("-DCONFIG_USB_KEYBOARD=1")
        # Add srouce files
        list(APPEND uboot_deps src/uboot/src/common/usb_kbd.c)
        list(APPEND uboot_deps src/uboot/src/drivers/input/keyboard-uclass.c)
    else()
        message(FATAL_ERROR "Unrecognised USB keyboard support level. Aborting.")
    endif()


    ######################################################
    # Complete configuration of source and include files #
    ######################################################

    # Pull in the base source files used by all platforms
    list(APPEND uboot_deps src/uboot/src/env/attr.c)
    list(APPEND uboot_deps src/uboot/src/env/callback.c)
    list(APPEND uboot_deps src/uboot/src/env/common.c)
    list(APPEND uboot_deps src/uboot/src/env/env.c)
    list(APPEND uboot_deps src/uboot/src/env/flags.c)
    file(GLOB_RECURSE glob_result src/uboot/src/lib/*.c)
    list(APPEND uboot_deps ${glob_result})
    list(APPEND uboot_deps src/uboot/src/cmd/blk_common.c)
    list(APPEND uboot_deps src/uboot/src/cmd/dm.c)
    list(APPEND uboot_deps src/uboot/src/cmd/nvedit.c)
    list(APPEND uboot_deps src/uboot/src/common/cli_readline.c)
    list(APPEND uboot_deps src/uboot/src/common/cli_simple.c)
    list(APPEND uboot_deps src/uboot/src/common/cli.c)
    list(APPEND uboot_deps src/uboot/src/common/command.c)
    list(APPEND uboot_deps src/uboot/src/common/fdt_support.c)
    list(APPEND uboot_deps src/uboot/src/common/stdio.c)
    list(APPEND uboot_deps src/uboot/src/drivers/block/blk-uclass.c)
    list(APPEND uboot_deps src/uboot/src/drivers/core/device-remove.c)
    list(APPEND uboot_deps src/uboot/src/drivers/core/device.c)
    list(APPEND uboot_deps src/uboot/src/drivers/core/devres.c)
    list(APPEND uboot_deps src/uboot/src/drivers/core/dump.c)
    list(APPEND uboot_deps src/uboot/src/drivers/core/fdtaddr.c)
    list(APPEND uboot_deps src/uboot/src/drivers/core/lists.c)
    list(APPEND uboot_deps src/uboot/src/drivers/core/of_access.c)
    list(APPEND uboot_deps src/uboot/src/drivers/core/of_addr.c)
    list(APPEND uboot_deps src/uboot/src/drivers/core/ofnode.c)
    list(APPEND uboot_deps src/uboot/src/drivers/core/read.c)
    list(APPEND uboot_deps src/uboot/src/drivers/core/root.c)
    list(APPEND uboot_deps src/uboot/src/drivers/core/simple-bus.c)
    list(APPEND uboot_deps src/uboot/src/drivers/core/uclass.c)
    list(APPEND uboot_deps src/uboot/src/drivers/phy/phy-uclass.c)

    file(GLOB_RECURSE wrapper_deps src/wrapper/*.c)
    file(GLOB_RECURSE stub_deps src/stub/src/*.c)
    file(GLOB_RECURSE plat_deps src/plat/${KernelPlatform}/*.c)

    # Add the 'uboot_helper.h' include to all U-Boot source files to provide
    # the required includes and configuration.
    set_source_files_properties(${uboot_deps} PROPERTIES COMPILE_FLAGS "-include uboot_helper.h")

    # Generate the final list of source files.
    list(APPEND deps ${wrapper_deps})
    list(APPEND deps ${uboot_deps})
    list(APPEND deps ${stub_deps})
    list(APPEND deps ${plat_deps})
    list(SORT deps)
    target_sources(ubootdrivers PRIVATE ${deps})

    target_include_directories(ubootdrivers PRIVATE
        include/stub/include
        include/wrapper
        src/uboot/src/lib
        include/uboot/include
    )

    # Add library speciifc includes based upon the platform.
    target_include_directories(ubootdrivers PRIVATE include/plat/${KernelPlatform})

    # Declare the public interface to the library.
    target_include_directories(ubootdrivers INTERFACE include/public_api)

    target_link_libraries(ubootdrivers sel4 muslc sel4platsupport utils)

    #########################
    # Manage logging levels #
    #########################

    # Provide a mapping between the seL4 logging level and the U-Boot logging level
    if("${LIB_UBOOT_LOGGING_LEVEL}" STREQUAL "ZF_LOG_VERBOSE")
        add_definitions("-DCONFIG_LOGLEVEL=9")
    elseif("${LIB_UBOOT_LOGGING_LEVEL}" STREQUAL "ZF_LOG_DEBUG")
        add_definitions("-DCONFIG_LOGLEVEL=8")
    elseif("${LIB_UBOOT_LOGGING_LEVEL}" STREQUAL "ZF_LOG_INFO")
        add_definitions("-DCONFIG_LOGLEVEL=6")
    elseif("${LIB_UBOOT_LOGGING_LEVEL}" STREQUAL "ZF_LOG_WARN")
        add_definitions("-DCONFIG_LOGLEVEL=4")
    elseif("${LIB_UBOOT_LOGGING_LEVEL}" STREQUAL "ZF_LOG_ERROR")
        add_definitions("-DCONFIG_LOGLEVEL=3")
    elseif("${LIB_UBOOT_LOGGING_LEVEL}" STREQUAL "ZF_LOG_FATAL")
        add_definitions("-DCONFIG_LOGLEVEL=0")
    else()
        message(FATAL_ERROR "No recognised logging level set in LIB_UBOOT_LOGGING_LEVEL. Aborting.")
    endif()

endif()
