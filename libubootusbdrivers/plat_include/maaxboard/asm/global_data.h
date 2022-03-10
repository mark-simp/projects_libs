/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * (C) Copyright 2002-2010
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 */

#ifndef	__ASM_GBL_DATA_H
#define __ASM_GBL_DATA_H

#ifndef __ASSEMBLY__

#include <config.h>

#include <asm/types.h>
#include <linux/types.h>

/* Architecture-specific global data */
struct arch_global_data {};

#include <asm-generic/global_data.h>

#define DECLARE_GLOBAL_DATA_PTR   extern gd_t *gd

#endif /* __ASSEMBLY__ */

#endif /* __ASM_GBL_DATA_H */