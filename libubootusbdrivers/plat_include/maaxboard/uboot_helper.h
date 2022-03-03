#pragma once

/* This file contains adaptors and definitions / symbols required by code
 * sourced from U-Boot.
 */

#define __LINUX_ARM_ARCH__			8
#define __KERNEL__

struct udevice;		/* Not clear why this is forward declaration is required */

#define CONFIG_ARM					true
#define CONFIG_ARM64   				true    /* TODO: Look at AARCH32 support */
#define CONFIG_LTO              	true    /* Don't use a register as a pointer to global_data */
#define CONFIG_DM               	true
#define CONFIG_DM_USB           	true
#define CONFIG_USB_XHCI_DWC3		true
#define CONFIG_DM_DEVICE_REMOVE		true
#define CONFIG_OF_LIBFDT			true
#define CONFIG_OF_LIVE   			true

#define CONFIG_DM_RESET         	false
#define CONFIG_DEVRES           	false
#define CONFIG_DM_DMA           	false
#define CONFIG_OF_PLATDATA			false
#define CONFIG_OF_PLATDATA_RT   	false
#define CONFIG_OF_PLATDATA_INST		false
#define CONFIG_OF_PLATDATA_NO_BIND	false
#define CONFIG_OF_PLATDATA_PARENT	false
#define CONFIG_OF_REAL          	false
#define CONFIG_OF_TRANSLATE			false
#define CONFIG_ACPIGEN          	false
#define CONFIG_PHYS_TO_BUS      	false
#define CONFIG_OF_CONTROL       	false
#define CONFIG_CLK    				false
#define CONFIG_VAL_SYS_MALLOC_F_LEN	false
#define CONFIG_DM_SEQ_ALIAS			false
#define CONFIG_POWER_DOMAIN			false
#define CONFIG_IOMMU				false

#define IS_ENABLED(OPTION)   		OPTION
#define CONFIG_IS_ENABLED(OPTION)   CONFIG_ ## OPTION
#define CONFIG_VAL(OPTION)  		CONFIG_VAL_ ## OPTION

#define CONFIG_ERR_PTR_OFFSET   	1024
#define CONFIG_NR_DRAM_BANKS		1
#define CONFIG_LINKER_LIST_ALIGN	4
#define CONFIG_SYS_CACHELINE_SIZE 	64 /* for cortex a53 MPCore */
#define ARCH_DMA_MINALIGN   		CONFIG_SYS_CACHELINE_SIZE
#define DO_DEPS_ONLY				true
#define errno_str(X)				""

#define mutex_init(...)
#define mutex_lock(...)
#define mutex_unlock(...)

#define likely(x)       __builtin_expect(!!(x), 1)
#define unlikely(x)     __builtin_expect(!!(x), 0)

#define __bitwise 		/*__attribute__((bitwise)) */
#define __force 		/* __attribute__((force)) */
#define __maybe_unused	/* __attribute__((maybe_unused)) */
#define __packed	    __attribute__((packed))
#define __weak   	    __attribute__((weak))
#define __must_check    __attribute__((warn_unused_result))
#define __aligned(x) 	__attribute__((aligned(x)))
#define __section(x) 	__attribute__((section(x)))
#define __always_inline	inline __attribute__((always_inline))
#define noinline		__attribute__((noinline))
#define __iomem			/* __attribute__((iomem)) */
#define __deprecated	__attribute__((deprecated))

#include <stddef.h>
#include <stdio.h>
#include <assert.h>
#include <uboot_print.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/byteorder/little_endian.h>
#include <linux/byteorder/generic.h>
#include <asm/barriers.h>
#include <asm/io.h>
