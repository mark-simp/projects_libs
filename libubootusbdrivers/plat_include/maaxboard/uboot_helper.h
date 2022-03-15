#pragma once

/*
 * This file contains adaptors and definitions / symbols required by code
 * sourced from U-Boot.
 */

#define __LINUX_ARM_ARCH__			8
#define __KERNEL__
#define __UBOOT__

// struct udevice;		/* Not clear why this is forward declaration is required */

#define CONFIG_ARM					        	true
#define CONFIG_ARM64   					        true  /* TODO: Look at AARCH32 support */

#define CONFIG_DM                          		true
#define CONFIG_OF_LIBFDT				        true
#define ENABLED_CONFIG_OF_LIVE   				true
#define ENABLED_CONFIG_OF_REAL          		true
#define ENABLED_CONFIG_OF_PLATDATA				false
#define ENABLED_CONFIG_OF_PLATDATA_RT   		false
#define ENABLED_CONFIG_OF_PLATDATA_DRIVER_RT	false
#define ENABLED_CONFIG_OF_PLATDATA_INST			false
#define ENABLED_CONFIG_OF_PLATDATA_NO_BIND		false
#define ENABLED_CONFIG_OF_PLATDATA_PARENT		false
#define ENABLED_CONFIG_OF_TRANSLATE	            false
#define ENABLED_CONFIG_SIMPLE_BUS_CORRECT_RANGE true
#define ENABLED_CONFIG_DM_USB           		true
#define ENABLED_CONFIG_PHY                      true
#define CONFIG_PHY                              true
#define ENABLED_CONFIG_USB_XHCI_DWC3			true
#define ENABLED_CONFIG_DM_DEVICE_REMOVE			true
#define CONFIG_USB_KEYBOARD                     true
#define CONFIG_USB_STORAGE                      true
#define CONFIG_USB_HOST                         true
#define ENABLED_CONFIG_USB_HOST                 true
#define CONFIG_USB_DWC3_GADGET                  true
#define ENABLED_CONFIG_USB_DWC3_GADGET          false
#define ENABLED_CONFIG_DM_RESET         		false /* Don't need to handle reset */
#define CONFIG_DEVRES           		        true
#define ENABLED_CONFIG_DM_DMA           		false
#define ENABLED_CONFIG_PHYS_TO_BUS      		false
#define ENABLED_CONFIG_OF_CONTROL       		false
#define ENABLED_CONFIG_DM_SEQ_ALIAS				false
#define ENABLED_CONFIG_POWER_DOMAIN				false /* No power domain driver support */
#define ENABLED_CONFIG_IOMMU					false /* No IOMMU driver support */
#define ENABLED_CONFIG_CLK    					false /* No clock driver support */
#define ENABLED_CONFIG_PCI						false /* No PCI support */
#define ENABLED_CONFIG_NEEDS_MANUAL_RELOC   	false
#define ENABLED_CONFIG_SIMPLE_BUS				false
#define ENABLED_CONFIG_LOG						false
#define ENABLED_CONFIG_OF_SEPARATE              false
#define ENABLED_CONFIG_OF_BOARD                 false
#define ENABLED_CONFIG_MULTI_DTB_FIT            false
#define ENABLED_CONFIG_SPL_SEPARATE_BSS         false
#define ENABLED_CONFIG_SPL_BUILD                false
#define ENABLED_CONFIG_PHANDLE_CHECK_SEQ        false

#define CONFIG_VAL_SYS_MALLOC_F_LEN		0
#define CONFIG_VAL_LOGLEVEL				0

#define IS_ENABLED(OPTION)   		ENABLED_ ## OPTION
#define CONFIG_IS_ENABLED(OPTION)   ENABLED_CONFIG_ ## OPTION
#define CONFIG_VAL(OPTION)  		CONFIG_VAL_ ## OPTION

#define CONFIG_ERR_PTR_OFFSET   	1024
#define CONFIG_NR_DRAM_BANKS		1
#define CONFIG_LINKER_LIST_ALIGN	4
#define CONFIG_SYS_CACHELINE_SIZE 	64 /* for cortex a53 MPCore */
#define ARCH_DMA_MINALIGN   		CONFIG_SYS_CACHELINE_SIZE
#define DO_DEPS_ONLY				true
#define errno_str(X)				""
#define CONFIG_SYS_HZ               1000
#define MAX_FILES                   3

#define mutex_init(...)
#define mutex_lock(...)
#define mutex_unlock(...)
#define EXPORT_SYMBOL(...)

#define likely(x)       __builtin_expect(!!(x), 1)
#define unlikely(x)     __builtin_expect(!!(x), 0)
#define assert_noisy(x)	assert(x)

#define __bitwise 		/*__attribute__((bitwise)) */
#define __force 		/* __attribute__((force)) */
#define __iomem			/* __attribute__((iomem)) */

#define _DEBUG			false

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <vsprintf.h>
#include <assert.h>
#include <time.h>
#include <uboot_print.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/byteorder/little_endian.h>
#include <linux/byteorder/generic.h>
#include <linux/compiler_types.h>
#include <linux/bitops.h>
#include <asm/barriers.h>
#include <asm/io.h>
#include <driver_data.h>
#include <command.h>
