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

static int fdt_sw_probe_(void *fdt)
{
	assert(false);
}

static int fdt_sw_probe_memrsv_(void *fdt)
{
	assert(false);
}

static int fdt_sw_probe_struct_(void *fdt)
{
	assert(false);
}

static inline uint32_t sw_flags(void *fdt)
{
	assert(false);
}

static void *fdt_grab_space_(void *fdt, size_t len)
{
	assert(false);
}

int fdt_create_with_flags(void *buf, int bufsize, uint32_t flags)
{
	assert(false);
}

int fdt_create(void *buf, int bufsize)
{
	assert(false);
}

int fdt_resize(void *fdt, void *buf, int bufsize)
{
	assert(false);
}

int fdt_add_reservemap_entry(void *fdt, uint64_t addr, uint64_t size)
{
	assert(false);
}

int fdt_finish_reservemap(void *fdt)
{
	assert(false);
}

int fdt_begin_node(void *fdt, const char *name)
{
	assert(false);
}

int fdt_end_node(void *fdt)
{
	assert(false);
}

static int fdt_add_string_(void *fdt, const char *s)
{
	assert(false);
}

static void fdt_del_last_string_(void *fdt, const char *s)
{
	assert(false);
}

static int fdt_find_add_string_(void *fdt, const char *s, int *allocated)
{
	assert(false);
}

int fdt_property_placeholder(void *fdt, const char *name, int len, void **valp)
{
	assert(false);
}

int fdt_property(void *fdt, const char *name, const void *val, int len)
{
	assert(false);
}

int fdt_finish(void *fdt)
{
	assert(false);
}
