#
# SPDX-License-Identifier: BSD-2-Clause
#

cmake_minimum_required(VERSION 3.7.2)

project(libubootusbdrivers C)

# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DZF_LOG_LEVEL=ZF_LOG_VERBOSE")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DZF_LOG_LEVEL=ZF_LOG_INFO")

config_option(LibUBOOTUSB LIB_UBOOT_USB "Build USB interface library" DEFAULT ON)

add_config_library(ubootusbdrivers "${configure_string}")
if(LibUBOOTUSB)

    # Set up standard configuration macros U-Boot code expects based upon
    # the architecture.
    if(KernelArchARM)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DCONFIG_ARM")
        if(Kernel64)
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DCONFIG_ARM64")
        endif()

        if(KernelArchArmV8a)
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__LINUX_ARM_ARCH__=8")
        elseif (KernelArchArmV7a OR KernelArchArmV7ve)
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__LINUX_ARM_ARCH__=7")
        endif()
    endif()

    file(
        GLOB
            deps
            src/*.c
            src/lib/*.c
            src/lib/libfdt/*.c
            src/stubs/*.c
            src/uboot/*.c
            src/uboot/cmd/*.c
            src/uboot/common/*.c
            src/uboot/drivers/block/*.c
            src/uboot/drivers/core/*.c
            src/uboot/drivers/input/*.c
            src/uboot/drivers/phy/*.c
            src/uboot/drivers/usb/common/*.c
            src/uboot/drivers/usb/dwc3/*.c
            src/uboot/drivers/usb/host/*.c
            src/uboot/drivers/usb/gadget/*.c
            src/uboot/env/*.c
            src/uboot/lib/*.c
            src/plat/${KernelPlatform}/*.c
    )

    list(SORT deps)

    add_library(ubootusbdrivers STATIC EXCLUDE_FROM_ALL ${deps})
    target_include_directories(ubootusbdrivers PUBLIC
        include
        src/lib
        uboot_include
        uboot_arch_include/${KernelArch}
        plat_include/${KernelPlatform}
    )
    target_link_libraries(ubootusbdrivers sel4 muslc sel4platsupport utils)
endif()
