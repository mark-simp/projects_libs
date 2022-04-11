#include <sel4platsupport/io.h>

#define MAX_DMA_ALLOCS 64

struct dma_allocation_t {
    bool in_use;
    void *vaddr;
    uintptr_t paddr;
    size_t size;
};

static struct dma_allocation_t dma_alloc[MAX_DMA_ALLOCS];

static ps_dma_man_t *sel4_dma_manager = NULL;


static int next_free_allocation_index(void)
{
    for (int x = 0; x < MAX_DMA_ALLOCS; x++)
        if (!dma_alloc[x].in_use) return x;
    return -1;
}

static int find_allocation_index_by_vaddr(void *vaddr)
{
    for (int x = 0; x < MAX_DMA_ALLOCS; x++)
        if (dma_alloc[x].in_use && dma_alloc[x].vaddr == vaddr) return x;
    return -1;
}

void sel4_dma_flush_range(unsigned long start, unsigned long stop)
{
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
        dma_alloc[alloc_index].vaddr,
        dma_alloc[alloc_index].size);

    // Memory allocated and pinned. Update bookkeeping.
    dma_alloc[alloc_index].in_use = false;
    dma_alloc[alloc_index].vaddr = 0;
    dma_alloc[alloc_index].paddr = 0;
    dma_alloc[alloc_index].size = 0;
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
        false,
        PS_MEM_NORMAL);
    if (vaddr == NULL)
    {
        ZF_LOGE("DMA allocation returned null pointer");
        return NULL;
    }

    uintptr_t paddr = sel4_dma_manager->dma_pin_fn(
        sel4_dma_manager->cookie,
        vaddr,
        size);
    if (!paddr)
    {
        ZF_LOGE("DMA pin return null pointer");
        // Clean up before returning.
        sel4_dma_manager->dma_free_fn(
            sel4_dma_manager->cookie,
            vaddr,
            size);
        return NULL;
    }

    ZF_LOGD(
        "size = 0x%x, align = 0x%x, vaddr = %p, paddr = %p, alloc_index = %i",
        size, align, vaddr, paddr, alloc_index);

    // Memory allocated and pinned. Update bookkeeping.
    dma_alloc[alloc_index].in_use = true;
    dma_alloc[alloc_index].vaddr = vaddr;
    dma_alloc[alloc_index].paddr = paddr;
    dma_alloc[alloc_index].size = size;

    return vaddr;
}

void* sel4_dma_malloc(size_t size)
{
    /* Default to alignment on cacheline boundaries */
    return sel4_dma_memalign(CONFIG_SYS_CACHELINE_SIZE, size);
}

void *sel4_dma_virt_to_phys(void *vaddr)
{
    assert(sel4_dma_manager != NULL);

    // Find the allocation containing this address.
    int alloc_index = -1;
    for (int x = 0; x < MAX_DMA_ALLOCS; x++)
        if (dma_alloc[x].in_use &&
            vaddr >= dma_alloc[x].vaddr &&
            vaddr < dma_alloc[x].vaddr + dma_alloc[x].size)
            {
                alloc_index = x;
                break;
            }
    if (alloc_index < 0)
    {
        ZF_LOGE("Unable to determine physical address from virtual %p", vaddr);
        for (int y = 0; y < MAX_DMA_ALLOCS; y++)
            if (dma_alloc[y].in_use)
                ZF_LOGE(" --> Index %i: vaddr = %p, size = 0x%x",
                    y, dma_alloc[y].vaddr, dma_alloc[y].size);
        /* This is a fatal error. Not being able to determine an address
         * indicates that we are attempting to communicate with the XHCI
         * device via memory that has not been mapped into the physical
         * address space. This implies that additional data needs to be
         * DMA allocated. */
        assert(false);
    }

    // Return the translated address
    return (void*) dma_alloc[alloc_index].paddr +
        (vaddr - dma_alloc[alloc_index].vaddr);
}

void *sel4_dma_phys_to_virt(void *paddr)
{
    assert(sel4_dma_manager != NULL);

    // Find the allocation containing this address.
    int alloc_index = -1;
    for (int x = 0; x < MAX_DMA_ALLOCS; x++)
        if (dma_alloc[x].in_use &&
            (uintptr_t)paddr >= dma_alloc[x].paddr &&
            (uintptr_t)paddr < dma_alloc[x].paddr + dma_alloc[x].size)
            {
                alloc_index = x;
                break;
            }
    if (alloc_index < 0)
    {
        ZF_LOGE("Unable to determine virtual address from physical %p", paddr);
        /* This is a fatal error. Not being able to determine an address
         * indicates that we are attempting to communicate with the XHCI
         * device via memory that has not been mapped into the physical
         * address space. This implies that additional data needs to be
         * DMA allocated. */
        assert(false);
    }

    // Return the translated address
    return (uintptr_t) dma_alloc[alloc_index].vaddr +
        (paddr - dma_alloc[alloc_index].paddr);
}

bool sel4_dma_is_virt_mapped(void *vaddr)
{
    assert(sel4_dma_manager != NULL);

    // Find the allocation containing this address.
    int alloc_index = -1;
    for (int x = 0; x < MAX_DMA_ALLOCS; x++)
        if (dma_alloc[x].in_use &&
            vaddr >= dma_alloc[x].vaddr &&
            vaddr < dma_alloc[x].vaddr + dma_alloc[x].size)
                return true;
    return false;
}

void sel4_dma_initialise(ps_dma_man_t *dma_manager)
{
    sel4_dma_manager = dma_manager;

    for (int x = 0; x < MAX_DMA_ALLOCS; x++)
    {
        dma_alloc[x].in_use = false;
        dma_alloc[x].vaddr = NULL;
        dma_alloc[x].paddr = 0;
        dma_alloc[x].size = 0;
    }
}

void sel4_dma_shutdown(void)
{
    // Deallocate any currently allocated DMA.
    for (int x = 0; x < MAX_DMA_ALLOCS; x++)
    {
        if (dma_alloc[x].in_use)
            sel4_dma_free(dma_alloc[x].vaddr);
    }

    // Clear the pointer to the DMA routines.
    sel4_dma_manager = NULL;
}