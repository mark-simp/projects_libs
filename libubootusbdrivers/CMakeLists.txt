#
# SPDX-License-Identifier: BSD-2-Clause
#

cmake_minimum_required(VERSION 3.7.2)

project(libubootusbdrivers C)

# Set the logging level for the library (e.g. ZF_LOG_INFO, ZF_LOG_VERBOSE, etc)
set(LIB_UBOOT_LOGGING_LEVEL "ZF_LOG_INFO")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DZF_LOG_LEVEL=${LIB_UBOOT_LOGGING_LEVEL}")

# config_option(LibUBOOTUSB LIB_UBOOT_USB "Build USB interface library" DEFAULT ON)

add_config_library(ubootusbdrivers "${configure_string}")
if(LibUBOOTUSB)

    # Set up standard configuration macros U-Boot code expects based upon
    # the architecture.
    if(KernelArchARM)
        # The architecture as ARM.
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DCONFIG_ARM")

        # Tell U-Boot code whether this is ARM32 or ARM64.
        if(Kernel64)
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DCONFIG_ARM64")
        endif()

        # Tell U-Boot code which generation of ARM we are using.
        if(KernelArchArmV8a)
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__LINUX_ARM_ARCH__=8")
        elseif(KernelArchArmV7a OR KernelArchArmV7ve)
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D__LINUX_ARM_ARCH__=7")
        endif()

        # Tell U-Boot code the processor's cacheline size.
        math(EXPR CACHELINE_SIZE "1 << ${KernelArmCacheLineSizeBits}")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DCONFIG_SYS_CACHELINE_SIZE=${CACHELINE_SIZE}")
    endif()

    # Provide a mapping between the seL4 logging level and the U-Boot logging level
    if("${LIB_UBOOT_LOGGING_LEVEL}" STREQUAL "ZF_LOG_VERBOSE")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DCONFIG_LOGLEVEL=9")
    elseif("${LIB_UBOOT_LOGGING_LEVEL}" STREQUAL "ZF_LOG_DEBUG")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DCONFIG_LOGLEVEL=8")
    elseif("${LIB_UBOOT_LOGGING_LEVEL}" STREQUAL "ZF_LOG_INFO")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DCONFIG_LOGLEVEL=6")
    elseif("${LIB_UBOOT_LOGGING_LEVEL}" STREQUAL "ZF_LOG_WARN")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DCONFIG_LOGLEVEL=4")
    elseif("${LIB_UBOOT_LOGGING_LEVEL}" STREQUAL "ZF_LOG_ERROR")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DCONFIG_LOGLEVEL=3")
    elseif("${LIB_UBOOT_LOGGING_LEVEL}" STREQUAL "ZF_LOG_FATAL")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DCONFIG_LOGLEVEL=0")
    else()
        error("No recognised logging level set in LIB_UBOOT_LOGGING_LEVEL. Aborting.")
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
