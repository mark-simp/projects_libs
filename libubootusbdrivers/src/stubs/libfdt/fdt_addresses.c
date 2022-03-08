// SPDX-License-Identifier: (GPL-2.0-or-later OR BSD-2-Clause)

#include <uboot_helper.h>

#include <linux/libfdt_env.h>
#include "fdt.h"
#include "libfdt.h"
#include "libfdt_internal.h"

static int fdt_cells(const void *fdt, int nodeoffset, const char *name)
{
	assert(false);
}

int fdt_address_cells(const void *fdt, int nodeoffset)
{
	assert(false);
}

int fdt_size_cells(const void *fdt, int nodeoffset)
{
	assert(false);
}

int fdt_appendprop_addrrange(void *fdt, int parent, int nodeoffset,
			     const char *name, uint64_t addr, uint64_t size)
{
	assert(false);
}
