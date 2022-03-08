// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (c) 2011 The Chromium OS Authors.
 */

#include <uboot_helper.h>

#include <common.h>
#include <dm.h>
#include <log.h>
#include <malloc.h>
// #include <net.h>
// #include <dm/of_extra.h>
#include <env.h>
#include <errno.h>
#include <fdtdec.h>
#include <fdt_support.h>
#include <linux/libfdt.h>
// #include <serial.h>
#include <asm/global_data.h>
// #include <asm/sections.h>
#include <linux/ctype.h>

const char *fdtdec_get_srcname(void)
{
    assert(false);
}

const char *fdtdec_get_compatible(enum fdt_compat_id id)
{
    assert(false);
}

fdt_addr_t fdtdec_get_addr_size_fixed(const void *blob, int node,
				      const char *prop_name, int index, int na,
				      int ns, fdt_size_t *sizep,
				      bool translate)
{
    assert(false);
}

fdt_addr_t fdtdec_get_addr_size_auto_parent(const void *blob, int parent,
					    int node, const char *prop_name,
					    int index, fdt_size_t *sizep,
					    bool translate)
{
    assert(false);
}

fdt_addr_t fdtdec_get_addr_size_auto_noparent(const void *blob, int node,
					      const char *prop_name, int index,
					      fdt_size_t *sizep,
					      bool translate)
{
    assert(false);
}

fdt_addr_t fdtdec_get_addr_size(const void *blob, int node,
				const char *prop_name, fdt_size_t *sizep)
{
    assert(false);
}

fdt_addr_t fdtdec_get_addr(const void *blob, int node, const char *prop_name)
{
    assert(false);
}

int fdtdec_get_pci_vendev(const void *blob, int node, u16 *vendor, u16 *device)
{
    assert(false);
}

int fdtdec_get_pci_bar32(const struct udevice *dev, struct fdt_pci_addr *addr,
			 u32 *bar)
{
    assert(false);
}

int fdtdec_get_pci_bus_range(const void *blob, int node,
			     struct fdt_resource *res)
{
    assert(false);
}

uint64_t fdtdec_get_uint64(const void *blob, int node, const char *prop_name,
			   uint64_t default_val)
{
    assert(false);
}

int fdtdec_get_is_enabled(const void *blob, int node)
{
    assert(false);
}

enum fdt_compat_id fdtdec_lookup(const void *blob, int node)
{
    assert(false);
}

int fdtdec_next_compatible(const void *blob, int node, enum fdt_compat_id id)
{
    assert(false);
}

int fdtdec_next_compatible_subnode(const void *blob, int node,
				   enum fdt_compat_id id, int *depthp)
{
    assert(false);
}

int fdtdec_next_alias(const void *blob, const char *name, enum fdt_compat_id id,
		      int *upto)
{
    assert(false);
}

int fdtdec_find_aliases_for_id(const void *blob, const char *name,
			       enum fdt_compat_id id, int *node_list,
			       int maxcount)
{
    assert(false);
}

int fdtdec_add_aliases_for_id(const void *blob, const char *name,
			      enum fdt_compat_id id, int *node_list,
			      int maxcount)
{
    assert(false);
}

int fdtdec_get_alias_seq(const void *blob, const char *base, int offset,
			 int *seqp)
{
    assert(false);
}

int fdtdec_get_alias_highest_id(const void *blob, const char *base)
{
    assert(false);
}

const char *fdtdec_get_chosen_prop(const void *blob, const char *name)
{
    assert(false);
}

int fdtdec_get_chosen_node(const void *blob, const char *name)
{
    assert(false);
}

int fdtdec_check_fdt(void)
{
    assert(false);
}

int fdtdec_prepare_fdt(void)
{
    assert(false);
}

int fdtdec_lookup_phandle(const void *blob, int node, const char *prop_name)
{
    assert(false);
}

static const void *get_prop_check_min_len(const void *blob, int node,
					  const char *prop_name, int min_len,
					  int *err)
{
    assert(false);
}

int fdtdec_get_int_array(const void *blob, int node, const char *prop_name,
			 u32 *array, int count)
{
    assert(false);
}

int fdtdec_get_int_array_count(const void *blob, int node,
			       const char *prop_name, u32 *array, int count)
{
    assert(false);
}

const u32 *fdtdec_locate_array(const void *blob, int node,
			       const char *prop_name, int count)
{
    assert(false);
}

int fdtdec_get_bool(const void *blob, int node, const char *prop_name)
{
    assert(false);
}

int fdtdec_parse_phandle_with_args(const void *blob, int src_node,
				   const char *list_name,
				   const char *cells_name,
				   int cell_count, int index,
				   struct fdtdec_phandle_args *out_args)
{
    assert(false);
}

int fdtdec_get_byte_array(const void *blob, int node, const char *prop_name,
			  u8 *array, int count)
{
    assert(false);
}

const u8 *fdtdec_locate_byte_array(const void *blob, int node,
				   const char *prop_name, int count)
{
    assert(false);
}

u64 fdtdec_get_number(const fdt32_t *ptr, unsigned int cells)
{
    assert(false);
}

int fdt_get_resource(const void *fdt, int node, const char *property,
		     unsigned int index, struct fdt_resource *res)
{
    assert(false);
}

int fdt_get_named_resource(const void *fdt, int node, const char *property,
			   const char *prop_names, const char *name,
			   struct fdt_resource *res)
{
    assert(false);
}

static int decode_timing_property(const void *blob, int node, const char *name,
				  struct timing_entry *result)
{
    assert(false);
}

int fdtdec_decode_display_timing(const void *blob, int parent, int index,
				 struct display_timing *dt)
{
    assert(false);
}

int fdtdec_setup_mem_size_base(void)
{
    assert(false);
}

ofnode get_next_memory_node(ofnode mem)
{
    assert(false);
}

int fdtdec_setup_memory_banksize(void)
{
    assert(false);
}

int fdtdec_setup_mem_size_base_lowest(void)
{
    assert(false);
}

static int uncompress_blob(const void *src, ulong sz_src, void **dstp)
{
    assert(false);
}

static void *fdt_find_separate(void)
{
    assert(false);


int fdtdec_set_ethernet_mac_address(void *fdt, const u8 *mac, size_t size)
{
    assert(false);
}

static int fdtdec_init_reserved_memory(void *blob)
{
    assert(false);
}

int fdtdec_add_reserved_memory(void *blob, const char *basename,
			       const struct fdt_memory *carveout,
			       const char **compatibles, unsigned int count,
			       uint32_t *phandlep, unsigned long flags)
{
    assert(false);
}

int fdtdec_get_carveout(const void *blob, const char *node,
			const char *prop_name, unsigned int index,
			struct fdt_memory *carveout, const char **name,
			const char ***compatiblesp, unsigned int *countp,
			unsigned long *flags)
{
    assert(false);
}

int fdtdec_set_carveout(void *blob, const char *node, const char *prop_name,
			unsigned int index, const struct fdt_memory *carveout,
			const char *name, const char **compatibles,
			unsigned int count, unsigned long flags)
{
    assert(false);
}

__weak int fdtdec_board_setup(const void *fdt_blob)
{
    assert(false);
}

static void setup_multi_dtb_fit(void)
{
    assert(false);
}

int fdtdec_setup(void)
{
    assert(false);
}

int fdtdec_resetup(int *rescan)
{
    assert(false);
}

int fdtdec_decode_ram_size(const void *blob, const char *area, int board_id,
			   phys_addr_t *basep, phys_size_t *sizep,
			   struct bd_info *bd)
{
    assert(false);
}
