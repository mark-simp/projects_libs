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
    endif()

    # Set up configuration macros based upon the platform
    if(KernelPlatImx8mq)
        add_definitions("-DCONFIG_ARCH_IMX8M=1")
        add_definitions("-DCONFIG_IMX8M=1")
        add_definitions("-DCONFIG_IMX8MQ=1")
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
    file(GLOB_RECURSE uboot_deps src/uboot/src/*.c)
    file(GLOB_RECURSE stub_deps src/stub/src/*.c)
    file(GLOB_RECURSE plat_deps src/plat/${KernelPlatform}/*.c)
    list(APPEND deps ${wrapper_deps})
    list(APPEND deps ${uboot_deps})
    list(APPEND deps ${stub_deps})
    list(APPEND deps ${plat_deps})

    # Enumerate the set of platform dependent osurce files
    if(KernelPlatImx8mq)
        file(GLOB_RECURSE uboot_arch_deps src/uboot/arch/imx8mq/*.c)
        list(APPEND deps ${uboot_arch_deps})
    else()
        set(uboot_arch_deps "")
    endif()

    # Add the 'uboot_helper.h' include to all U-Boot source files to provide
    # the required includes and configuration.
    set_source_files_properties(${uboot_deps} PROPERTIES COMPILE_FLAGS "-include uboot_helper.h")
    set_source_files_properties(${uboot_arch_deps} PROPERTIES COMPILE_FLAGS "-include uboot_helper.h")

    list(SORT deps)

    add_library(ubootdrivers STATIC EXCLUDE_FROM_ALL ${deps})

    target_include_directories(ubootdrivers PRIVATE
        include/wrapper
        src/uboot/src/lib
        include/uboot/include
        include/stub/include
    )

    # Add U-Boot prviate inclues based upon the architecture.
    if(KernelArchARM)
        target_include_directories(ubootdrivers PRIVATE include/uboot/arch/arm)
        target_include_directories(ubootdrivers PRIVATE include/stub/arch/arm)
        target_include_directories(ubootdrivers PRIVATE include/uboot/arch/proc-armv)
        if(KernelArchArmV8a)
            target_include_directories(ubootdrivers PRIVATE include/uboot/arch/armv8)
        endif()
    endif()

    # Add U-Boot prviate inclues based upon the platform.
    if(KernelPlatImx8mq)
        target_include_directories(ubootdrivers PRIVATE include/uboot/arch/imx8m)
    endif()

    # Add library speciifc includes based upon the platform.
    target_include_directories(ubootdrivers PRIVATE include/plat/${KernelPlatform})

    # Declare the public interface to the library.
    target_include_directories(ubootdrivers INTERFACE include/public_api)

    target_link_libraries(ubootdrivers sel4 muslc sel4platsupport utils)
endif()
