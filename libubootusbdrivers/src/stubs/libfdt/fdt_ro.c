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

static int fdt_nodename_eq_(const void *fdt, int offset,
			    const char *s, int len)
{
	assert(false);
}

const char *fdt_get_string(const void *fdt, int stroffset, int *lenp)
{
	assert(false);
}

const char *fdt_string(const void *fdt, int stroffset)
{
	assert(false);
}

static int fdt_string_eq_(const void *fdt, int stroffset,
			  const char *s, int len)
{
	assert(false);
}

int fdt_find_max_phandle(const void *fdt, uint32_t *phandle)
{
	assert(false);
}

int fdt_generate_phandle(const void *fdt, uint32_t *phandle)
{
	assert(false);
}

static const struct fdt_reserve_entry *fdt_mem_rsv(const void *fdt, int n)
{
	assert(false);
}

int fdt_get_mem_rsv(const void *fdt, int n, uint64_t *address, uint64_t *size)
{
	assert(false);
}

int fdt_num_mem_rsv(const void *fdt)
{
	assert(false);
}

static int nextprop_(const void *fdt, int offset)
{
	assert(false);
}

int fdt_subnode_offset_namelen(const void *fdt, int offset,
			       const char *name, int namelen)
{
	assert(false);
}

int fdt_subnode_offset(const void *fdt, int parentoffset,
		       const char *name)
{
	assert(false);
}

int fdt_path_offset_namelen(const void *fdt, const char *path, int namelen)
{
	assert(false);
}

int fdt_path_offset(const void *fdt, const char *path)
{
	assert(false);
}

const char *fdt_get_name(const void *fdt, int nodeoffset, int *len)
{
	assert(false);
}

int fdt_first_property_offset(const void *fdt, int nodeoffset)
{
	assert(false);
}

int fdt_next_property_offset(const void *fdt, int offset)
{
	assert(false);
}

static const struct fdt_property *fdt_get_property_by_offset_(const void *fdt,
						              int offset,
						              int *lenp)
{
	assert(false);
}

const struct fdt_property *fdt_get_property_by_offset(const void *fdt,
						      int offset,
						      int *lenp)
{
	assert(false);
}

static const struct fdt_property *fdt_get_property_namelen_(const void *fdt,
						            int offset,
						            const char *name,
						            int namelen,
							    int *lenp,
							    int *poffset)
{
	assert(false);
}


const struct fdt_property *fdt_get_property_namelen(const void *fdt,
						    int offset,
						    const char *name,
						    int namelen, int *lenp)
{
	assert(false);
}


const struct fdt_property *fdt_get_property(const void *fdt,
					    int nodeoffset,
					    const char *name, int *lenp)
{
	assert(false);
}

const void *fdt_getprop_namelen(const void *fdt, int nodeoffset,
				const char *name, int namelen, int *lenp)
{
	assert(false);
}

const void *fdt_getprop_by_offset(const void *fdt, int offset,
				  const char **namep, int *lenp)
{
	assert(false);
}

const void *fdt_getprop(const void *fdt, int nodeoffset,
			const char *name, int *lenp)
{
	assert(false);
}

uint32_t fdt_get_phandle(const void *fdt, int nodeoffset)
{
	assert(false);
}

const char *fdt_get_alias_namelen(const void *fdt,
				  const char *name, int namelen)
{
	assert(false);
}

const char *fdt_get_alias(const void *fdt, const char *name)
{
	assert(false);
}

int fdt_get_path(const void *fdt, int nodeoffset, char *buf, int buflen)
{
	assert(false);
}

int fdt_supernode_atdepth_offset(const void *fdt, int nodeoffset,
				 int supernodedepth, int *nodedepth)
{
	assert(false);
}

int fdt_node_depth(const void *fdt, int nodeoffset)
{
	assert(false);
}

int fdt_parent_offset(const void *fdt, int nodeoffset)
{
	assert(false);
}

int fdt_node_offset_by_prop_value(const void *fdt, int startoffset,
				  const char *propname,
				  const void *propval, int proplen)
{
	assert(false);
}

int fdt_node_offset_by_phandle(const void *fdt, uint32_t phandle)
{
	assert(false);
}

int fdt_stringlist_contains(const char *strlist, int listlen, const char *str)
{
	assert(false);
}

int fdt_stringlist_count(const void *fdt, int nodeoffset, const char *property)
{
	assert(false);
}

int fdt_stringlist_search(const void *fdt, int nodeoffset, const char *property,
			  const char *string)
{
	assert(false);
}

const char *fdt_stringlist_get(const void *fdt, int nodeoffset,
			       const char *property, int idx,
			       int *lenp)
{
	assert(false);
}

int fdt_node_check_compatible(const void *fdt, int nodeoffset,
			      const char *compatible)
{
	assert(false);
}

int fdt_node_offset_by_compatible(const void *fdt, int startoffset,
				  const char *compatible)
{
	assert(false);
}

int fdt_check_full(const void *fdt, size_t bufsize)
{
	assert(false);

}
