#pragma once

/* This file contains adaptors and definitions / symbols required by code
 * sourced from U-Boot.
 */

// TODO: Need to remove this
#define CONFIG_IS_ENABLED(X)   false

#define __ALIGN_MASK(x, mask)	(((x) + (mask)) & ~(mask))
#define UBOOT_ALIGN(x, a)		__ALIGN_MASK((x), (typeof(x))(a) - 1)

#define BUG_ON(X)  assert(!(X))
#define BUG()  assert(false)

#define __bitwise /*__attribute__((bitwise))*/
#define __force /* __attribute__((force)) */
#define __packed	__attribute__((packed))

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef int64_t s64;
typedef int32_t s32;
typedef int16_t s16;
typedef int8_t  s8;

typedef unsigned long ulong;
typedef unsigned short ushort;
typedef unsigned int  uint;
typedef unsigned char uchar;

typedef u64 __u64;
typedef u32 __u32;
typedef u16 __u16;
typedef u8  __u8;

typedef s64 __bitwise __le64;
typedef s32 __bitwise __le32;
typedef s16 __bitwise __le16;
typedef s8  __bitwise __le8;

typedef s64 __bitwise __be64;
typedef s32 __bitwise __be32;
typedef s16 __bitwise __be16;
typedef s8  __bitwise __be8;

#define U8_MAX		((u8)~0U)
#define S8_MAX		((s8)(U8_MAX>>1))
#define S8_MIN		((s8)(-S8_MAX - 1))
#define U16_MAX		((u16)~0U)
#define S16_MAX		((s16)(U16_MAX>>1))
#define S16_MIN		((s16)(-S16_MAX - 1))
#define U32_MAX		((u32)~0U)
#define S32_MAX		((s32)(U32_MAX>>1))
#define S32_MIN		((s32)(-S32_MAX - 1))
#define U64_MAX		((u64)~0ULL)
#define S64_MAX		((s64)(U64_MAX>>1))
#define S64_MIN		((s64)(-S64_MAX - 1))

#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })
