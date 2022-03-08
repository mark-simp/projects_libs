#
# SPDX-License-Identifier: BSD-2-Clause
#

cmake_minimum_required(VERSION 3.7.2)

project(libubootusbdrivers C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DZF_LOG_LEVEL=ZF_LOG_VERBOSE")

config_option(LibUBOOTUSB LIB_UBOOT_USB "Build USB interface library" DEFAULT ON)

add_config_library(ubootusbdrivers "${configure_string}")
if(LibUBOOTUSB)

    file(
        GLOB
            deps
            src/*.c
            src/stubs/*.c
            src/stubs/libfdt/*.c
            src/uboot/*.c
            src/uboot/common/*.c
            src/uboot/drivers/core/*.c
            src/uboot/drivers/usb/dwc3/*.c
            src/uboot/drivers/usb/host/*.c
            src/plat/${KernelPlatform}/*.c
    )

    list(SORT deps)

    add_library(ubootusbdrivers STATIC EXCLUDE_FROM_ALL ${deps})
    target_include_directories(ubootusbdrivers PUBLIC include
        src/stubs
        include
        uboot_include
        uboot_arch_include/${KernelArch}
        plat_include/${KernelPlatform}
    )
    target_link_libraries(ubootusbdrivers sel4 muslc platsupport ubootusbdrivers_Config)
endif()
