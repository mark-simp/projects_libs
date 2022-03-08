// SPDX-License-Identifier: (GPL-2.0-or-later OR BSD-2-Clause)
/*
 * libfdt - Flat Device Tree manipulation
 * Copyright (C) 2006 David Gibson, IBM Corporation.
 */
#include <uboot_helper.h>

#include "libfdt_env.h"

#include <fdt.h>
#include <libfdt.h>

#include "libfdt_internal.h"

static int fdt_blocks_misordered_(const void *fdt,
				  int mem_rsv_size, int struct_size)
{
	assert(false);
}

static int fdt_rw_probe_(void *fdt)
{
	assert(false);
}

static inline int fdt_data_size_(void *fdt)
{
	assert(false);
}

static int fdt_splice_(void *fdt, void *splicepoint, int oldlen, int newlen)
{
	assert(false);
}

static int fdt_splice_mem_rsv_(void *fdt, struct fdt_reserve_entry *p,
			       int oldn, int newn)
{
	assert(false);
}

static int fdt_splice_struct_(void *fdt, void *p,
			      int oldlen, int newlen)
{
	assert(false);
}

static void fdt_del_last_string_(void *fdt, const char *s)
{
	assert(false);
}

static int fdt_splice_string_(void *fdt, int newlen)
{
	assert(false);
}

static int fdt_find_add_string_(void *fdt, const char *s, int *allocated)
{
	assert(false);
}

int fdt_add_mem_rsv(void *fdt, uint64_t address, uint64_t size)
{
	assert(false);
}

int fdt_del_mem_rsv(void *fdt, int n)
{
	assert(false);
}

static int fdt_resize_property_(void *fdt, int nodeoffset, const char *name,
				int len, struct fdt_property **prop)
{
	assert(false);
}

static int fdt_add_property_(void *fdt, int nodeoffset, const char *name,
			     int len, struct fdt_property **prop)
{
	assert(false);
}

int fdt_set_name(void *fdt, int nodeoffset, const char *name)
{
	assert(false);
}

int fdt_setprop_placeholder(void *fdt, int nodeoffset, const char *name,
			    int len, void **prop_data)
{
	assert(false);
}

int fdt_setprop(void *fdt, int nodeoffset, const char *name,
		const void *val, int len)
{
	assert(false);
}

int fdt_appendprop(void *fdt, int nodeoffset, const char *name,
		   const void *val, int len)
{
	assert(false);
}

int fdt_delprop(void *fdt, int nodeoffset, const char *name)
{
	assert(false);
}

int fdt_add_subnode_namelen(void *fdt, int parentoffset,
			    const char *name, int namelen)
{
	assert(false);
}

int fdt_add_subnode(void *fdt, int parentoffset, const char *name)
{
	assert(false);
}

int fdt_del_node(void *fdt, int nodeoffset)
{
	assert(false);
}

static void fdt_packblocks_(const char *old, char *new,
			    int mem_rsv_size, int struct_size)
{
	assert(false);
}

int fdt_open_into(const void *fdt, void *buf, int bufsize)
{
	assert(false);
}

int fdt_pack(void *fdt)
{
	assert(false);
}
