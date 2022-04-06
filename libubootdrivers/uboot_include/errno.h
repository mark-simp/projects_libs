/* Minimal stub replacement of 'errno' */
#ifndef _ERRNO_H
#define _ERRNO_H

#include <linux/errno.h>

extern int errno_uboot;

#define __set_errno(val) do { errno_uboot = val; } while (0)

static const char error_message[] = "";

static inline const char *errno_str(int errno) { return error_message; }

#endif /* _ERRNO_H */