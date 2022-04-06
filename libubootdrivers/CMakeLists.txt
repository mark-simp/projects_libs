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

    # Set up standard configuration macros U-Boot code expects based upon
    # the architecture.
    if(KernelArchARM)
        # The architecture as ARM.
        add_definitions("-DCONFIG_ARM")

        # Tell U-Boot code whether this is ARM32 or ARM64.
        if(Kernel64)
            add_definitions("-DCONFIG_ARM64")
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
    endif()

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

    # Enumerate the set of source files
    file(GLOB_RECURSE wrapper_deps src/wrapper/*.c)
    file(GLOB_RECURSE uboot_deps src/uboot/*.c)
    file(GLOB_RECURSE plat_deps src/plat/${KernelPlatform}/*.c)
    list(APPEND deps ${wrapper_deps})
    list(APPEND deps ${uboot_deps})
    list(APPEND deps ${plat_deps})
    list(SORT deps)

    # Add the 'uboot_helper.h' include to all U-Boot source files to provide
    # the required includes and configuration.
    set_source_files_properties(${uboot_deps} PROPERTIES COMPILE_FLAGS "-include uboot_helper.h")

    add_library(ubootdrivers STATIC EXCLUDE_FROM_ALL ${deps})

    target_include_directories(ubootdrivers PUBLIC
        include/wrapper
        src/uboot/lib
        include/uboot/include
        include/uboot/arch/${KernelArch}
        include/plat/${KernelPlatform}
    )
    target_link_libraries(ubootdrivers sel4 muslc sel4platsupport utils)
endif()
