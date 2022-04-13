/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_DMA_MAPPING_H
#define _LINUX_DMA_MAPPING_H

#include <asm/cache.h>
#include <linux/dma-direction.h>
#include <linux/types.h>
#include <cpu_func.h>

static inline void *dma_alloc_coherent(size_t len, unsigned long *handle)
{
	*handle = (unsigned long)sel4_dma_memalign(ARCH_DMA_MINALIGN, ROUND(len, ARCH_DMA_MINALIGN));
	return (void *)*handle;
}

static inline void dma_free_coherent(void *addr)
{
	sel4_dma_free(addr);
}

static inline bool dma_mapping_error(struct udevice *dev, dma_addr_t addr)
{
	return !sel4_dma_is_virt_mapped((void *) addr);
}

static inline dma_addr_t dma_map_single(void *vaddr, size_t len,
					enum dma_data_direction dir)
{
	unsigned long addr = (unsigned long) sel4_dma_malloc(len);

	len = ALIGN(len, ARCH_DMA_MINALIGN);

	if (dir == DMA_FROM_DEVICE)
		sel4_dma_invalidate_range(addr, addr + len);
	else
		memcpy((void*) addr, vaddr, len);
		sel4_dma_flush_range(addr, addr + len);

	return addr;
}

static inline void dma_unmap_single(dma_addr_t addr, size_t len,
				    enum dma_data_direction dir)
{
	sel4_dma_flush_range(addr, addr + len);
	sel4_dma_copy_to_virt_and_free((void *) addr);
}

#endif