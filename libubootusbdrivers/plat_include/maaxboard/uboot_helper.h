#pragma once

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <uboot_print.h>
#include <linux/types.h>

/* This file contains adaptors and definitions / symbols required by code
 * sourced from U-Boot.
 */

typedef u64 phys_addr_t;
typedef u64 phys_size_t;
typedef u64 dma_addr_t;

#define __LINUX_ARM_ARCH__			8

#define CONFIG_ARM					true
#define CONFIG_ARM64   				true    /* TODO: Look at AARCH32 support */
#define CONFIG_LTO              	true    /* Don't use a register as a pointer to global_data */
#define CONFIG_DM               	true
#define CONFIG_DM_USB           	true
#define CONFIG_USB_XHCI_DWC3		true

#define CONFIG_DM_RESET         	false
#define CONFIG_DEVRES           	false
#define CONFIG_DM_DMA           	false
#define CONFIG_OF_PLATDATA_RT   	false
#define CONFIG_OF_REAL          	false
#define CONFIG_ACPIGEN          	false
#define CONFIG_PHYS_TO_BUS      	false
#define CONFIG_OF_CONTROL       	false
#define CONFIG_CLK    				false
#define CONFIG_VAL_SYS_MALLOC_F_LEN	false

#define CONFIG_IS_ENABLED(OPTION)   CONFIG_ ## OPTION
#define CONFIG_VAL(OPTION)  		CONFIG_VAL_ ## OPTION

#define CONFIG_ERR_PTR_OFFSET   	1024
#define CONFIG_NR_DRAM_BANKS		1
#define CONFIG_LINKER_LIST_ALIGN	4
#define DO_DEPS_ONLY				true

#define BUG_ON(X)  assert(!(X))
#define BUG()  assert(false)

#define likely(x)       __builtin_expect(!!(x), 1)
#define unlikely(x)     __builtin_expect(!!(x), 0)

#define __bitwise 		/*__attribute__((bitwise))*/
#define __force 		/* __attribute__((force)) */
#define __maybe_unused	/* __attribute__((maybe_unused)) */
#define __packed	    __attribute__((packed))
#define __weak   	    __attribute__((weak))
#define __must_check    __attribute__((warn_unused_result))
#define __aligned(x) 	__attribute__((aligned(x)))
#define __section(x) 	__attribute__((section(x)))
#define __always_inline	inline __attribute__((always_inline))
#define noinline		__attribute__((noinline))

#include <uboot_io.h>
