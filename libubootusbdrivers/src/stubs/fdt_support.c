// SPDX-License-Identifier: GPL-2.0+

#include <uboot_helper.h>

#include <dm.h>
#include <fdt_support.h>

u32 fdt_getprop_u32_default_node(const void *fdt, int off, int cell,
				const char *prop, const u32 dflt)
{
    assert(false);
}

u32 fdt_getprop_u32_default(const void *fdt, const char *path,
				const char *prop, const u32 dflt)
{
    assert(false);
}

int fdt_find_and_setprop(void *fdt, const char *node, const char *prop,
			 const void *val, int len, int create)
{
    assert(false);
}

int fdt_find_or_add_subnode(void *fdt, int parentoffset, const char *name)
{
    assert(false);
}

static int fdt_fixup_stdout(void *fdt, int chosenoff)
{
    assert(false);
}

static inline int fdt_setprop_uxx(void *fdt, int nodeoffset, const char *name,
				  uint64_t val, int is_u64)
{
    assert(false);
}

int fdt_root(void *fdt)
{
    assert(false);
}

int fdt_initrd(void *fdt, ulong initrd_start, ulong initrd_end)
{
    assert(false);
}

__weak char *board_fdt_chosen_bootargs(void)
{
    assert(false);
}

int fdt_chosen(void *fdt)
{
    assert(false);
}

void do_fixup_by_path(void *fdt, const char *path, const char *prop,
		      const void *val, int len, int create)
{
    assert(false);
}

void do_fixup_by_path_u32(void *fdt, const char *path, const char *prop,
			  u32 val, int create)
{
    assert(false);
}

void do_fixup_by_prop(void *fdt,
		      const char *pname, const void *pval, int plen,
		      const char *prop, const void *val, int len,
		      int create)
{
    assert(false);
}

void do_fixup_by_prop_u32(void *fdt,
			  const char *pname, const void *pval, int plen,
			  const char *prop, u32 val, int create)
{
    assert(false);
}

void do_fixup_by_compat(void *fdt, const char *compat,
			const char *prop, const void *val, int len, int create)
{
    assert(false);
}

void do_fixup_by_compat_u32(void *fdt, const char *compat,
			    const char *prop, u32 val, int create)
{
    assert(false);
}

int fdt_fixup_memory(void *blob, u64 start, u64 size)
{
    assert(false);
}

void fdt_fixup_ethernet(void *fdt)
{
    assert(false);
}

int fdt_record_loadable(void *blob, u32 index, const char *name,
			uintptr_t load_addr, u32 size, uintptr_t entry_point,
			const char *type, const char *os, const char *arch)
{
    assert(false);
}

int fdt_shrink_to_minimum(void *blob, uint extrasize)
{
    assert(false);
}

int fdt_delete_disabled_nodes(void *blob)
{
    assert(false);
}

int fdt_increase_size(void *fdt, int add_len)
{
    assert(false);
}

void fdt_del_node_and_alias(void *blob, const char *alias)
{
    assert(false);
}

static void of_dump_addr(const char *s, const fdt32_t *addr, int na) {
    assert(false);
}

void fdt_support_default_count_cells(const void *blob, int parentoffset,
					int *addrc, int *sizec)
{
    assert(false);
}

static u64 of_bus_default_map(fdt32_t *addr, const fdt32_t *range,
		int na, int ns, int pna)
{
    assert(false);
}

static int of_bus_default_translate(fdt32_t *addr, u64 offset, int na)
{
    assert(false);
}

static struct of_bus *of_match_bus(const void *blob, int parentoffset)
{
    assert(false);
}

static int of_translate_one(const void *blob, int parent, struct of_bus *bus,
			    struct of_bus *pbus, fdt32_t *addr,
			    int na, int ns, int pna, const char *rprop)
{
    assert(false);
}

u64 fdt_translate_address(const void *blob, int node_offset,
			  const fdt32_t *in_addr)
{
    assert(false);
}

u64 fdt_translate_dma_address(const void *blob, int node_offset,
			      const fdt32_t *in_addr)
{
    assert(false);
}

int fdt_get_dma_range(const void *blob, int node, phys_addr_t *cpu,
		      dma_addr_t *bus, u64 *size)
{
    assert(false);
}

int fdt_node_offset_by_compat_reg(void *blob, const char *compat,
					phys_addr_t compat_off)
{
    assert(false);
}

static int vnode_offset_by_pathf(void *blob, const char *fmt, va_list ap)
{
    assert(false);
}

int fdt_node_offset_by_pathf(void *blob, const char *fmt, ...)
{
    assert(false);
}

int fdt_set_phandle(void *fdt, int nodeoffset, uint32_t phandle)
{
    assert(false);
}

unsigned int fdt_create_phandle(void *fdt, int nodeoffset)
{
    assert(false);
}

unsigned int fdt_create_phandle_by_compatible(void *fdt, const char *compat)
{
    assert(false);
}

unsigned int fdt_create_phandle_by_pathf(void *fdt, const char *fmt, ...)
{
    assert(false);
}

int fdt_set_node_status(void *fdt, int nodeoffset, enum fdt_status status)
{
    assert(false);
}

int fdt_set_status_by_alias(void *fdt, const char* alias,
			    enum fdt_status status)
{
    assert(false);
}

int fdt_set_status_by_compatible(void *fdt, const char *compat,
				 enum fdt_status status)
{
    assert(false);
}

int fdt_set_status_by_pathf(void *fdt, enum fdt_status status, const char *fmt,
			    ...)
{
    assert(false);
}

int fdt_verify_alias_address(void *fdt, int anode, const char *alias, u64 addr)
{
    assert(false);
}

u64 fdt_get_base_address(const void *fdt, int node)
{
    assert(false);
}

static int fdt_read_prop(const fdt32_t *prop, int prop_len, int cell_off,
			 uint64_t *val, int cells)
{
    assert(false);
}

int fdt_read_range(void *fdt, int node, int n, uint64_t *child_addr,
		   uint64_t *addr, uint64_t *len)
{
    assert(false);
}

int fdt_setup_simplefb_node(void *fdt, int node, u64 base_address, u32 width,
			    u32 height, u32 stride, const char *format)
{
    assert(false);
}

int fdt_fixup_display(void *blob, const char *path, const char *display)
{
    assert(false);
}

int fdt_valid(struct fdt_header **blobp)
{
    assert(false);
}
