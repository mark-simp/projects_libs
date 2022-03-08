// SPDX-License-Identifier: (GPL-2.0-or-later OR BSD-2-Clause)

#include <uboot_helper.h>

#include <linux/libfdt_env.h>
#include "fdt.h"
#include "libfdt.h"
#include "libfdt_internal.h"

int fdt_setprop_inplace_namelen_partial(void *fdt, int nodeoffset,
					const char *name, int namelen,
					uint32_t idx, const void *val,
					int len)
{
	assert(false);
}

int fdt_setprop_inplace(void *fdt, int nodeoffset, const char *name,
			const void *val, int len)
{
	assert(false);
}

static void fdt_nop_region_(void *start, int len)
{
	assert(false);
}

int fdt_nop_property(void *fdt, int nodeoffset, const char *name)
{
	assert(false);
}

int fdt_node_end_offset_(void *fdt, int offset)
{
	assert(false);
}

int fdt_nop_node(void *fdt, int nodeoffset)
{
	assert(false);
}
