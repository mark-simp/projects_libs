#pragma once

#include <stddef.h>
#include <string.h>
#include <uboot_io.h>
#include <uboot_print.h>

/* This file contains adaptors and definitions / symbols required by code
 * sourced from U-Boot.
 */

#define CONFIG_ARM				true
#define CONFIG_ARM64   			true    /* TODO: Look at AARCH32 support */
#define CONFIG_DM               true
#define CONFIG_DM_USB           true
#define CONFIG_USB_XHCI_DWC3	true

#define CONFIG_DM_RESET         false
#define CONFIG_DEVRES           false
#define CONFIG_DM_DMA           false
#define CONFIG_OF_PLATDATA_RT   false
#define CONFIG_OF_REAL          false
#define CONFIG_ACPIGEN          false
#define CONFIG_PHYS_TO_BUS      false
#define CONFIG_OF_CONTROL       false
#define CONFIG_CLK              false

#define CONFIG_IS_ENABLED(OPTION)   CONFIG_ ## OPTION

#define CONFIG_ERR_PTR_OFFSET   1024

#define CONFIG_NR_DRAM_BANKS	1

// #define __ALIGN_MASK(x, mask)	(((x) + (mask)) & ~(mask))
// #define UBOOT_ALIGN(x, a)		__ALIGN_MASK((x), (typeof(x))(a) - 1)

#define BUG_ON(X)  assert(!(X))
#define BUG()  assert(false)

#define likely(x)       __builtin_expect(!!(x), 1)
#define unlikely(x)     __builtin_expect(!!(x), 0)

#define of_live_active()    false

#define __bitwise /*__attribute__((bitwise))*/
#define __force /* __attribute__((force)) */
#define __packed	    __attribute__((packed))
#define __must_check    __attribute__((warn_unused_result))
#define __aligned(x) 	__attribute__((aligned(x)))
#define __section(x) 	__attribute__((section(x)))

// typedef uint64_t u64;
// typedef uint32_t u32;
// typedef uint16_t u16;
// typedef uint8_t  u8;

// typedef int64_t s64;
// typedef int32_t s32;
// typedef int16_t s16;
// typedef int8_t  s8;

// typedef unsigned long ulong;
// typedef unsigned short ushort;
// typedef unsigned int  uint;
// typedef unsigned char uchar;

// typedef u64 __u64;
// typedef u32 __u32;
// typedef u16 __u16;
// typedef u8  __u8;

// typedef s64 __bitwise __le64;
// typedef s32 __bitwise __le32;
// typedef s16 __bitwise __le16;
// typedef s8  __bitwise __le8;

// typedef s64 __bitwise __be64;
// typedef s32 __bitwise __be32;
// typedef s16 __bitwise __be16;
// typedef s8  __bitwise __be8;

// #define U8_MAX		((u8)~0U)
// #define S8_MAX		((s8)(U8_MAX>>1))
// #define S8_MIN		((s8)(-S8_MAX - 1))
// #define U16_MAX		((u16)~0U)
// #define S16_MAX		((s16)(U16_MAX>>1))
// #define S16_MIN		((s16)(-S16_MAX - 1))
// #define U32_MAX		((u32)~0U)
// #define S32_MAX		((s32)(U32_MAX>>1))
// #define S32_MIN		((s32)(-S32_MAX - 1))
// #define U64_MAX		((u64)~0ULL)
// #define S64_MAX		((s64)(U64_MAX>>1))
// #define S64_MIN		((s64)(-S64_MAX - 1))

// /*
//  * min()/max()/clamp() macros that also do
//  * strict type-checking.. See the
//  * "unnecessary" pointer comparison.
//  */
// #define min(x, y) ({				\
// 	typeof(x) _min1 = (x);			\
// 	typeof(y) _min2 = (y);			\
// 	(void) (&_min1 == &_min2);		\
// 	_min1 < _min2 ? _min1 : _min2; })

// #define max(x, y) ({				\
// 	typeof(x) _max1 = (x);			\
// 	typeof(y) _max2 = (y);			\
// 	(void) (&_max1 == &_max2);		\
// 	_max1 > _max2 ? _max1 : _max2; })

// /**
//  * upper_32_bits - return bits 32-63 of a number
//  * @n: the number we're accessing
//  *
//  * A basic shift-right of a 64- or 32-bit quantity.  Use this to suppress
//  * the "right shift count >= width of type" warning when that quantity is
//  * 32-bits.
//  */
// #define upper_32_bits(n) ((u32)(((n) >> 16) >> 16))

// /**
//  * lower_32_bits - return bits 0-31 of a number
//  * @n: the number we're accessing
//  */
// #define lower_32_bits(n) ((u32)(n))

// typedef u64 dma_addr_t;
// typedef u64 phys_addr_t;

// static inline void *phys_to_virt(phys_addr_t paddr)
// {
// 	return (void *)(unsigned long)paddr;
// }

// static inline phys_addr_t virt_to_phys(void *vaddr)
// {
// 	return (phys_addr_t)((unsigned long)vaddr);
// }