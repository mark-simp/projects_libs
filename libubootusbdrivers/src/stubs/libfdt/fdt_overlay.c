// SPDX-License-Identifier: (GPL-2.0-or-later OR BSD-2-Clause)

#include <uboot_helper.h>

#include <linux/libfdt_env.h>
#include "fdt.h"
#include "libfdt.h"
#include "libfdt_internal.h"

static uint32_t overlay_get_target_phandle(const void *fdto, int fragment)
{
	assert(false);
}

static int overlay_get_target(const void *fdt, const void *fdto,
			      int fragment, char const **pathp)
{
	assert(false);
}

static int overlay_phandle_add_offset(void *fdt, int node,
				      const char *name, uint32_t delta)
{
	assert(false);
}

static int overlay_adjust_node_phandles(void *fdto, int node,
					uint32_t delta)
{
	assert(false);
}

static int overlay_adjust_local_phandles(void *fdto, uint32_t delta)
{
	assert(false);
}

static int overlay_update_local_node_references(void *fdto,
						int tree_node,
						int fixup_node,
						uint32_t delta)
{
	assert(false);
}

static int overlay_update_local_references(void *fdto, uint32_t delta)
{
	assert(false);
}

static int overlay_fixup_one_phandle(void *fdt, void *fdto,
				     int symbols_off,
				     const char *path, uint32_t path_len,
				     const char *name, uint32_t name_len,
				     int poffset, const char *label)
{
	assert(false);
};

static int overlay_fixup_phandle(void *fdt, void *fdto, int symbols_off,
				 int property)
{
	assert(false);
}

static int overlay_fixup_phandles(void *fdt, void *fdto)
{
	assert(false);
}

static int overlay_apply_node(void *fdt, int target,
			      void *fdto, int node)
{
	assert(false);
}

static int overlay_merge(void *fdt, void *fdto)
{
	assert(false);

}

static int get_path_len(const void *fdt, int nodeoffset)
{
	assert(false);

}

static int overlay_symbol_update(void *fdt, void *fdto)
{
	assert(false);
}

int fdt_overlay_apply(void *fdt, void *fdto)
{
	assert(false);
}

int fdt_overlay_apply_node(void *fdt, int target, void *fdto, int node)
{
	assert(false);
}
