/*
 * Unaligned acces routines.
*/

static inline u16 __get_unaligned_le16(const u8 *p)
{
	return p[0] | p[1] << 8;
}

static inline u32 __get_unaligned_le32(const u8 *p)
{
	return p[0] | p[1] << 8 | p[2] << 16 | p[3] << 24;
}

static inline u64 __get_unaligned_le64(const u8 *p)
{
	return (u64)__get_unaligned_le32(p + 4) << 32 |
	       __get_unaligned_le32(p);
}

static inline void __put_unaligned_le16(u16 val, u8 *p)
{
	*p++ = val;
	*p++ = val >> 8;
}

static inline void __put_unaligned_le32(u32 val, u8 *p)
{
	__put_unaligned_le16(val >> 16, p + 2);
	__put_unaligned_le16(val, p);
}

static inline void __put_unaligned_le64(u64 val, u8 *p)
{
	__put_unaligned_le32(val >> 32, p + 4);
	__put_unaligned_le32(val, p);
}

static inline u16 get_unaligned_le16(const void *p)
{
	return __get_unaligned_le16((const u8 *)p);
}

static inline u32 get_unaligned_le32(const void *p)
{
	return __get_unaligned_le32((const u8 *)p);
}

static inline u64 get_unaligned_le64(const void *p)
{
	return __get_unaligned_le64((const u8 *)p);
}

static inline void put_unaligned_le16(u16 val, void *p)
{
	__put_unaligned_le16(val, p);
}

static inline void put_unaligned_le32(u32 val, void *p)
{
	__put_unaligned_le32(val, p);
}

static inline void put_unaligned_le64(u64 val, void *p)
{
	__put_unaligned_le64(val, p);
}

static inline u16 __get_unaligned_be16(const u8 *p)
{
	return p[0] << 8 | p[1];
}

static inline u32 __get_unaligned_be32(const u8 *p)
{
	return p[0] << 24 | p[1] << 16 | p[2] << 8 | p[3];
}

static inline u64 __get_unaligned_be64(const u8 *p)
{
	return (u64)__get_unaligned_be32(p) << 32 |
	       __get_unaligned_be32(p + 4);
}

static inline void __put_unaligned_be16(u16 val, u8 *p)
{
	*p++ = val >> 8;
	*p++ = val;
}

static inline void __put_unaligned_be32(u32 val, u8 *p)
{
	__put_unaligned_be16(val >> 16, p);
	__put_unaligned_be16(val, p + 2);
}

static inline void __put_unaligned_be64(u64 val, u8 *p)
{
	__put_unaligned_be32(val >> 32, p);
	__put_unaligned_be32(val, p + 4);
}

static inline u16 get_unaligned_be16(const void *p)
{
	return __get_unaligned_be16((const u8 *)p);
}

static inline u32 get_unaligned_be32(const void *p)
{
	return __get_unaligned_be32((const u8 *)p);
}

static inline u64 get_unaligned_be64(const void *p)
{
	return __get_unaligned_be64((const u8 *)p);
}

static inline void put_unaligned_be16(u16 val, void *p)
{
	__put_unaligned_be16(val, p);
}

static inline void put_unaligned_be32(u32 val, void *p)
{
	__put_unaligned_be32(val, p);
}

static inline void put_unaligned_be64(u64 val, void *p)
{
	__put_unaligned_be64(val, p);
}

/*
 * Cause a link-time error if we try an unaligned access other than
 * 1,2,4 or 8 bytes long
 */
extern void __bad_unaligned_access_size(void);

#define __get_unaligned_le(ptr) ((__force typeof(*(ptr)))({                  \
    __builtin_choose_expr(sizeof(*(ptr)) == 1, *(ptr),                       \
    __builtin_choose_expr(sizeof(*(ptr)) == 2, get_unaligned_le16((ptr)),    \
    __builtin_choose_expr(sizeof(*(ptr)) == 4, get_unaligned_le32((ptr)),    \
    __builtin_choose_expr(sizeof(*(ptr)) == 8, get_unaligned_le64((ptr)),    \
    __bad_unaligned_access_size()))));                                       \
    }))

#define __get_unaligned_be(ptr) ((__force typeof(*(ptr)))({                  \
    __builtin_choose_expr(sizeof(*(ptr)) == 1, *(ptr),                       \
    __builtin_choose_expr(sizeof(*(ptr)) == 2, get_unaligned_be16((ptr)),    \
    __builtin_choose_expr(sizeof(*(ptr)) == 4, get_unaligned_be32((ptr)),    \
    __builtin_choose_expr(sizeof(*(ptr)) == 8, get_unaligned_be64((ptr)),    \
    __bad_unaligned_access_size()))));                                       \
    }))

#define __put_unaligned_le(val, ptr) ({                        \
    void *__gu_p = (ptr);                                      \
    switch (sizeof(*(ptr))) {                                  \
    case 1:                                                    \
        *(u8 *)__gu_p = (__force u8)(val);                     \
        break;                                                 \
    case 2:                                                    \
        put_unaligned_le16((__force u16)(val), __gu_p);        \
        break;                                                 \
    case 4:                                                    \
        put_unaligned_le32((__force u32)(val), __gu_p);        \
        break;                                                 \
    case 8:                                                    \
        put_unaligned_le64((__force u64)(val), __gu_p);        \
        break;                                                 \
    default:                                                   \
        __bad_unaligned_access_size();                         \
        break;                                                 \
    }                                                          \
    (void)0; })

#define __put_unaligned_be(val, ptr) ({                        \
    void *__gu_p = (ptr);                                      \
    switch (sizeof(*(ptr))) {                                  \
    case 1:                                                    \
        *(u8 *)__gu_p = (__force u8)(val);                     \
        break;                                                 \
    case 2:                                                    \
        put_unaligned_be16((__force u16)(val), __gu_p);        \
        break;                                                 \
    case 4:                                                    \
        put_unaligned_be32((__force u32)(val), __gu_p);        \
        break;                                                 \
    case 8:                                                    \
        put_unaligned_be64((__force u64)(val), __gu_p);        \
        break;                                                 \
    default:                                                   \
        __bad_unaligned_access_size();                         \
        break;                                                 \
    }                                                          \
    (void)0; })

#if __BYTE_ORDER == __LITTLE_ENDIAN
#define get_unaligned    __get_unaligned_le
#define put_unaligned    __put_unaligned_le
#else
#define get_unaligned    __get_unaligned_be
#define put_unaligned    __put_unaligned_be
#endif
