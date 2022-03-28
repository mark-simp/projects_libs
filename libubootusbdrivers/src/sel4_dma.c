#include <sel4platsupport/io.h>

#define MAX_DMA_ALLOCS 64

struct dma_allocation_t {
    bool in_use;
    void* vaddr;
    void* paddr;
    size_t size;
};

struct dma_allocation_t dma_allocation[MAX_DMA_ALLOCS];

ps_dma_man_t *sel4_dma_manager = NULL;


int next_free_allocation_index(void)
{
    for (int x = 0; x < MAX_DMA_ALLOCS; x++)
        if (!dma_allocation[x].in_use) return x;
    return -1;
}

int find_allocation_index_by_vaddr(void *vaddr)
{
    for (int x = 0; x < MAX_DMA_ALLOCS; x++)
        if (dma_allocation[x].in_use && dma_allocation[x].vaddr == vaddr) return x;
    return -1;
}

int find_allocation_index_by_paddr(void *paddr)
{
    for (int x = 0; x < MAX_DMA_ALLOCS; x++)
        if (dma_allocation[x].in_use && dma_allocation[x].paddr == paddr) return x;
    return -1;
}

void sel4_dma_initialise(ps_dma_man_t dma_manager)
{
    sel4_dma_manager = &dma_manager;

    for (int x = 0; x < MAX_DMA_ALLOCS; x++)
    {
        dma_allocation[x].in_use = false;
        dma_allocation[x].vaddr = NULL;
        dma_allocation[x].paddr = NULL;
        dma_allocation[x].size = 0;
    }
}

void sel4_dma_flush_range(unsigned long start, unsigned long stop)
{
    // ZF_LOGD("start = 0x%x, stop = 0x%x", start, stop);

    assert(sel4_dma_manager != NULL);

    size_t flush_size;
    if (stop > start)
        flush_size = (size_t) stop - start;
    else if (stop == start)
        flush_size = (size_t) 4;
    else
        return;

    sel4_dma_manager->dma_cache_op_fn(
        sel4_dma_manager->cookie,
        (void *) start,
        flush_size,
        DMA_CACHE_OP_CLEAN);
}

void sel4_dma_invalidate_range(unsigned long start, unsigned long stop)
{
    // ZF_LOGD("start = 0x%x, stop = 0x%x", start, stop);

    assert(sel4_dma_manager != NULL);

    sel4_dma_manager->dma_cache_op_fn(
        sel4_dma_manager->cookie,
        (void *) start,
        (size_t) stop - start + 4,
        DMA_CACHE_OP_INVALIDATE);
}

void sel4_dma_free(void *vaddr)
{
    assert(sel4_dma_manager != NULL);

    // Find the previous allocation.
    int alloc_index = find_allocation_index_by_vaddr(vaddr);
    if (alloc_index < 0)
    {
        ZF_LOGE("Call to free DMA allocation not in bookkeeping");
        return;
    }

    ZF_LOGD("vaddr = %p, alloc_index = %i", vaddr, alloc_index);

    sel4_dma_manager->dma_free_fn(
        sel4_dma_manager->cookie,
        dma_allocation[alloc_index].vaddr,
        dma_allocation[alloc_index].size);

    // Memory allocated and pinned. Update bookkeeping.
    dma_allocation[alloc_index].in_use = false;
    dma_allocation[alloc_index].vaddr = NULL;
    dma_allocation[alloc_index].paddr = NULL;
    dma_allocation[alloc_index].size = 0;
}

void* sel4_dma_memalign(size_t align, size_t size)
{
    assert(sel4_dma_manager != NULL);

    int alloc_index = next_free_allocation_index();
    if (alloc_index < 0)
    {
        ZF_LOGE("No free DMA allocation slots, unable to allocate");
        return NULL;
    }

    void* vaddr = sel4_dma_manager->dma_alloc_fn(
        sel4_dma_manager->cookie,
        size,
        align,
        false, // Don't cache
        PS_MEM_NORMAL);
    if (vaddr == NULL)
    {
        ZF_LOGE("DMA allocation return null pointer");
        return NULL;
    }

    void* paddr = sel4_dma_manager->dma_pin_fn(
        sel4_dma_manager->cookie,
        vaddr,
        size);
    if (vaddr == NULL)
    {
        ZF_LOGE("DMA pin return null pointer");
        // Clean up before returning.
        sel4_dma_manager->dma_free_fn(
            sel4_dma_manager->cookie,
            vaddr,
            size);
        return NULL;
    }

    ZF_LOGD("size = 0x%x, align = 0x%x, vaddr = %p, paddr = %p, alloc_index = %i", size, align, vaddr, paddr, alloc_index);

    // Memory allocated and pinned. Update bookkeeping.
    dma_allocation[alloc_index].in_use = true;
    dma_allocation[alloc_index].vaddr = vaddr;
    dma_allocation[alloc_index].paddr = paddr;
    dma_allocation[alloc_index].size = size;

    return vaddr;
}

void* sel4_dma_malloc(size_t size)
{
    return sel4_dma_memalign(4, size);
}

void* sel4_dma_virt_to_phys(void* vaddr)
{
    assert(sel4_dma_manager != NULL);

    // Find the previous allocation.
    int alloc_index = find_allocation_index_by_vaddr(vaddr);
    if (alloc_index < 0)
    {
        ZF_LOGE("Unable to determine physical address from virtual");
        return (uintptr_t) NULL;
    }

    ZF_LOGD("vaddr = %p -> paddr = %p", vaddr, dma_allocation[alloc_index].paddr);

    return (uintptr_t) dma_allocation[alloc_index].paddr;
}

void* sel4_dma_phys_to_virt(void *paddr)
{
    assert(sel4_dma_manager != NULL);

    // Find the previous allocation.
    int alloc_index = find_allocation_index_by_paddr(paddr);
    if (alloc_index < 0)
    {
        ZF_LOGE("Unable to determine virtual address from physical");
        return (uintptr_t) NULL;
    }

    ZF_LOGD("paddr = %p -> vaddr = %p", paddr, dma_allocation[alloc_index].vaddr);

    return (uintptr_t) dma_allocation[alloc_index].vaddr;
}