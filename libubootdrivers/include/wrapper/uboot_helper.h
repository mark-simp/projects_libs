#pragma once

/*
 * This file contains the base U-Boot configuration macros that must be in
 * place for the driver library and the base set of includes that are required
 * to cleanly compile the U-Boot source files.
 */

/* Enable seL4 specific code modifications to the U-Boot source files */
#define CONFIG_SEL4

/* Base defines expected by U-Boot */
#define __KERNEL__
#define __UBOOT__

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

/* Enable use of U-Boot's Driver Model (DM) driver framework */
#define CONFIG_DM                       1
#define CONFIG_DM_DEVICE_REMOVE			1

/* Enable use of the 'live' Open Firmware (OF) device tree */
#define CONFIG_OF_LIBFDT				1
#define CONFIG_OF_LIVE   				1
#define CONFIG_OF_REAL          		1
#define CONFIG_OF_CONTROL          		1

/* Enable use of the U-Boot command line interface */
#define CONFIG_CMDLINE                  1
#define CONFIG_SYS_MAXARGS              64 /* Max number of args in a command */
#define CONFIG_SYS_CBSIZE               256 /* Size of the console (input) buffer */
#define CONFIG_SYS_PBSIZE               256 /* Size of the print (ouput) buffer */
#define CONFIG_SYS_PROMPT               "" /* Command prompt */

/* Environment variable settings */
#define CONFIG_ENV_IS_NOWHERE           1
#define CONFIG_ENV_SIZE                 0x2000 /* 8K of env variable storage */

/* Settings related to standard input / output files */
#define CONFIG_SYS_DEVICE_NULLDEV       1 /* Define null stdio devices */
#define MAX_FILES   	                3 /* stdin, stdout and stderr */
#define UBOOT_STDIN 		            0 /* Enumerate the stdin file */
#define UBOOT_STDOUT    		        1 /* Enumerate the stdout file */
#define UBOOT_STDERR	    	        2 /* Enumerate the stderr file */

/* Base settings for drivers */
#define CONFIG_PHY                      1 /* Allow use of PHY drivers */
#define CONFIG_BLK                      1 /* Allow use of block device drivers */
#define CONFIG_DEVRES           		1 /* Allow device resource management */

/* Disable mutex support. Library is not intended to be thread-safe */
#define mutex_init(...)
#define mutex_lock(...)
#define mutex_unlock(...)

/* Disable symbols not required / supported */
#define EXPORT_SYMBOL(...)
#define __bitwise 		/* __attribute__((bitwise)) */
#define __force 		/* __attribute__((force)) */
#define __iomem			/* __attribute__((iomem)) */

/* Assorted macros needed to keep U-Boot source code happy */
#define CONFIG_ERR_PTR_OFFSET   	0
#define CONFIG_NR_DRAM_BANKS		0 /* Not used */
#define CONFIG_LMB_MEMORY_REGIONS   0 /* Not used */
#define CONFIG_LMB_RESERVED_REGIONS 0 /* Not used */
#define _DEBUG			            false /* Don't force debug logging */
#define CONFIG_SYS_HZ               1000 /* System, e.g. console, update rate */

/* Helper macros to wrap U-Boot 'print' routines on to seL4 equivalents */
#include <uboot_print.h>

/* Undefine the ALIGN macro to allow U-Boot code to redefine as required */
#undef ALIGN

/* Include headers expected by all U-Boot source code */
#include <plat_uboot_config.h>
#include <linux/kconfig.h>
#include <common.h>
#include <stdlib.h>
#include <string.h>
#include <vsprintf.h>
#include <assert.h>
#include <sel4_dma.h>
#include <driver_data.h>

/* Replace uses of U-Boot's lldiv function with the equivalent from muslc */
#define lldiv(A, B)     lldiv(A, B).quot