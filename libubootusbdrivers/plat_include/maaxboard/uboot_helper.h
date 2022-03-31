#pragma once

/*
 * This file contains adaptors and definitions / symbols required by code
 * sourced from U-Boot.
 */

#define __KERNEL__
#define __UBOOT__

/* Enable seL4 specific modifications */
#define CONFIG_SEL4

/* Define which routines the muslc library provides for us. Those not provided
 * are instead provided by U-Boot's string.c */
#define __HAVE_ARCH_STRCPY
#define __HAVE_ARCH_STRNCPY
#define __HAVE_ARCH_STRLCPY
#define __HAVE_ARCH_STRCAT
#define __HAVE_ARCH_STRNCAT
#define __HAVE_ARCH_STRLCAT
#define __HAVE_ARCH_STRCMP
#define __HAVE_ARCH_STRNCMP
#define __HAVE_ARCH_STRCHR
#define __HAVE_ARCH_STRRCHR
#define __HAVE_ARCH_STRSTR
#define __HAVE_ARCH_STRLEN
#define __HAVE_ARCH_STRNLEN
#define __HAVE_ARCH_STRDUP
#define __HAVE_ARCH_MEMSET
#define __HAVE_ARCH_MEMCPY
#define __HAVE_ARCH_MEMMOVE
#define __HAVE_ARCH_MEMCMP
#define __HAVE_ARCH_MEMCHR

/* TODO: This set should be being set by CMake based upon arch */
#define __LINUX_ARM_ARCH__			    8
#define CONFIG_ARM
#define CONFIG_ARM64

#define CONFIG_DM                       1
#define CONFIG_DM_USB                   1
#define CONFIG_DM_DEVICE_REMOVE			1

#define CONFIG_CMDLINE                  1

#define CONFIG_OF_LIBFDT				1
#define CONFIG_OF_LIVE   				1
#define CONFIG_OF_REAL          		1
#define CONFIG_OF_CONTROL          		1

#define CONFIG_SIMPLE_BUS_CORRECT_RANGE 1
#define CONFIG_PHY                      1
#define CONFIG_USB_XHCI_DWC3			1
#define CONFIG_USB_KEYBOARD             1
#define CONFIG_USB_STORAGE              1
#define CONFIG_USB_HOST                 1
#define CONFIG_USB_XHCI_HCD             1
#define CONFIG_DEVRES           		1

#define CONFIG_BLK                      1

#define CONFIG_LOGLEVEL				0
#define CONFIG_ERR_PTR_OFFSET   	0
#define CONFIG_NR_DRAM_BANKS		0
#define CONFIG_SYS_CACHELINE_SIZE 	64 /* for cortex a53 MPCore */
#define CONFIG_SYS_MAXARGS          64
#define CONFIG_SYS_CBSIZE           256
#define CONFIG_SYS_PBSIZE           256
#define CONFIG_SYS_PROMPT           ""
#define CONFIG_ENV_SIZE             0x2000
#define ARCH_DMA_MINALIGN   		CONFIG_SYS_CACHELINE_SIZE
#define errno_str(X)				""
#define CONFIG_SYS_HZ               1000
#define UBOOT_STDIN		            0
#define UBOOT_STDOUT		        1
#define UBOOT_STDERR		        2
#define MAX_FILES	                3

#define mutex_init(...)
#define mutex_lock(...)
#define mutex_unlock(...)
#define EXPORT_SYMBOL(...)

#define __bitwise 		/* __attribute__((bitwise)) */
#define __force 		/* __attribute__((force)) */
#define __iomem			/* __attribute__((iomem)) */

#define _DEBUG			false

#include <linux/kconfig.h>
#include <common.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <vsprintf.h>
#include <assert.h>
#include <time.h>
#include <sel4_dma.h>
#include <uboot_print.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/byteorder/little_endian.h>
#include <linux/byteorder/generic.h>
#include <linux/compiler_types.h>
#include <linux/compiler.h>
#include <linux/bitops.h>
#include <asm/barriers.h>
#include <asm/io.h>
#include <driver_data.h>
#include <command.h>
#include <exports.h>