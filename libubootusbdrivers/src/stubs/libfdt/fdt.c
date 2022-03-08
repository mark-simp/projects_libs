// SPDX-License-Identifier: (GPL-2.0-or-later OR BSD-2-Clause)

#include <uboot_helper.h>

#include <linux/libfdt_env.h>
#include "fdt.h"
#include "libfdt.h"
#include "libfdt_internal.h"

int32_t fdt_ro_probe_(const void *fdt)
{
	assert(false);
}

static int check_off_(uint32_t hdrsize, uint32_t totalsize, uint32_t off)
{
	assert(false);
}

static int check_block_(uint32_t hdrsize, uint32_t totalsize,
			uint32_t base, uint32_t size)
{
	assert(false);
}

size_t fdt_header_size_(uint32_t version)
{
	assert(false);
}

size_t fdt_header_size(const void *fdt)
{
	assert(false);
}

int fdt_check_header(const void *fdt)
{
	assert(false);
}

const void *fdt_offset_ptr(const void *fdt, int offset, unsigned int len)
{
	assert(false);
}

uint32_t fdt_next_tag(const void *fdt, int startoffset, int *nextoffset)
{
	assert(false);
}

int fdt_check_node_offset_(const void *fdt, int offset)
{
	assert(false);
}

int fdt_check_prop_offset_(const void *fdt, int offset)
{
	assert(false);
}

int fdt_next_node(const void *fdt, int offset, int *depth)
{
	assert(false);
}

int fdt_first_subnode(const void *fdt, int offset)
{
	assert(false);
}

int fdt_next_subnode(const void *fdt, int offset)
{
	assert(false);
}

const char *fdt_find_string_(const char *strtab, int tabsize, const char *s)
{
	assert(false);
}

int fdt_move(const void *fdt, void *buf, int bufsize)
{
	assert(false);
}
