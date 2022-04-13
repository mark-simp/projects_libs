#include <sel4platsupport/io.h>
#include <linux/dma-direction.h>

#define MAX_DMA_ALLOCS 64

struct dma_allocation_t {
    /* Base data for all DMA allocations */
    bool in_use;
    bool is_mapping;
    void *vaddr;
    uintptr_t paddr;
    size_t size;
    /* Additional data relevant only to DMA mappings */
    void *mapped_vaddr;
    enum dma_data_direction mapping_dir;
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

static int find_allocation_index_by_mapped_vaddr(void *mapped_vaddr)
{
    for (int x = 0; x < MAX_DMA_ALLOCS; x++)
        if (dma_alloc[x].in_use && dma_alloc[x].mapped_vaddr == mapped_vaddr) return x;
    return -1;
}

static int find_allocation_index_by_paddr(uintptr_t paddr)
{
    for (int x = 0; x < MAX_DMA_ALLOCS; x++)
        if (dma_alloc[x].in_use && dma_alloc[x].paddr == paddr) return x;
    return -1;
}

static void clear_allocation(int alloc_index)
{
    dma_alloc[alloc_index].in_use = false;
    dma_alloc[alloc_index].is_mapping = false;
    dma_alloc[alloc_index].vaddr = NULL;
    dma_alloc[alloc_index].paddr = 0;
    dma_alloc[alloc_index].size = 0;
    dma_alloc[alloc_index].mapped_vaddr = NULL;
    dma_alloc[alloc_index].mapping_dir = DMA_NONE;
}

void *sel4_dma_phys_to_virt(void *paddr)
{
    assert(sel4_dma_manager != NULL);

    // Find the allocation containing this address.
    for (int x = 0; x < MAX_DMA_ALLOCS; x++)
        if (dma_alloc[x].in_use &&
            (uintptr_t)paddr >= dma_alloc[x].paddr &&
            (uintptr_t)paddr < dma_alloc[x].paddr + dma_alloc[x].size) {
                if (dma_alloc[x].is_mapping)
                    return (uintptr_t) dma_alloc[x].mapped_vaddr +
                        (paddr - dma_alloc[x].paddr);
                else
                    return (uintptr_t) dma_alloc[x].vaddr +
                        (paddr - dma_alloc[x].paddr);
            }

    ZF_LOGE("Unable to determine virtual address from physical %p", paddr);
    /* This is a fatal error. Not being able to determine an address
        * indicates that we are attempting to communicate with a
        * device via memory that has not been mapped into the physical
        * address space. This implies that additional data needs to be
        * DMA allocated. */
    assert(false);
}

static int address_in_allocation_index(void *vaddr)
{
    // Find the allocation containing this address.
    for (int x = 0; x < MAX_DMA_ALLOCS; x++)
        if (dma_alloc[x].in_use) {
            if (!dma_alloc[x].is_mapping &&
                vaddr >= dma_alloc[x].vaddr &&
                vaddr < dma_alloc[x].vaddr + dma_alloc[x].size)
                return x;
            if (dma_alloc[x].is_mapping &&
                vaddr >= dma_alloc[x].mapped_vaddr &&
                vaddr < dma_alloc[x].mapped_vaddr + dma_alloc[x].size)
                return x;
        }
    return -1;
}

bool sel4_dma_is_virt_mapped(void *vaddr)
{
    assert(sel4_dma_manager != NULL);

    return (address_in_allocation_index(vaddr) >= 0);
}

void sel4_dma_flush_range(void *start, void *stop)
{
    assert(sel4_dma_manager != NULL);

    /* Only support cases where both the start and end address are addresses
     * we have previously allocated and mapped, and are in the same allocation */
    int start_alloc_index = address_in_allocation_index((void*) start);
    int stop_alloc_index = address_in_allocation_index((void*) start);

    if (start_alloc_index < 0) {
        ZF_LOGD("Flushed start address is not DMA allocated: %p", start);
        return;
    }
    if (stop_alloc_index < 0) {
        ZF_LOGD("Flushed stop address is not DMA allocated: %p", start);
        return;
    }
    if (start_alloc_index != stop_alloc_index) {
        ZF_LOGD("Flushed start and stop addresses span different DMA allocations: %p - %p", start, stop);
    }

    /* If this is mapped in the 'to device' direction then we need to start by
     * copying the mapped virtual data to the DMA-backed area before flushing */
    if (dma_alloc[start_alloc_index].is_mapping &&
        dma_alloc[start_alloc_index].mapping_dir == DMA_TO_DEVICE)
            memcpy(
                dma_alloc[start_alloc_index].vaddr,
                dma_alloc[start_alloc_index].mapped_vaddr,
                dma_alloc[start_alloc_index].size);

    /* Determine how much data to flush */
    size_t flush_size;
    if (stop > start)
        flush_size = (size_t) (stop - start);
    else if (stop == start)
        /* Default to flushing one cache line */
        flush_size = (size_t) CONFIG_SYS_CACHELINE_SIZE;
    else
        return;

    /* Determine the address to flush from */
    void* flush_start;
    if (dma_alloc[start_alloc_index].is_mapping)
        flush_start = dma_alloc[start_alloc_index].vaddr +
            ((void*) start - dma_alloc[start_alloc_index].mapped_vaddr);
    else
        flush_start = (void*) start;

    /* Perform the flush */
    sel4_dma_manager->dma_cache_op_fn(
        sel4_dma_manager->cookie,
        flush_start,
        flush_size,
        DMA_CACHE_OP_CLEAN);

    /* If this is mapped in the 'from device' direction then we need to finish by
     * copying the mapped virtual data to the DMA-backed area before flushing */
    if (dma_alloc[start_alloc_index].is_mapping &&
        dma_alloc[start_alloc_index].mapping_dir == DMA_FROM_DEVICE)
            memcpy(
                dma_alloc[start_alloc_index].mapped_vaddr,
                dma_alloc[start_alloc_index].vaddr,
                dma_alloc[start_alloc_index].size);
}

void sel4_dma_invalidate_range(void *start, void *stop)
{
    assert(sel4_dma_manager != NULL);

    /* Only support cases where both the start and end address are addresses
     * we have previously allocated and mapped, and are in the same allocation */
    int start_alloc_index = address_in_allocation_index((void*) start);
    int stop_alloc_index = address_in_allocation_index((void*) start);

    if (start_alloc_index < 0) {
        ZF_LOGD("Invalidate start address is not DMA allocated: %p", start);
        return;
    }
    if (stop_alloc_index < 0) {
        ZF_LOGD("Invalidate stop address is not DMA allocated: %p", start);
        return;
    }
    if (start_alloc_index != stop_alloc_index) {
        ZF_LOGD("Invalidate start and stop addresses span different DMA allocations: %p - %p", start, stop);
    }

    /* Determine how much data to invalidate */
    size_t inval_size;
    if (stop > start)
        inval_size = (size_t) (stop - start);
    else if (stop == start)
        /* Default to flushing one cache line */
        inval_size = (size_t) CONFIG_SYS_CACHELINE_SIZE;
    else
        return;

    /* Determine the address to invalidate from */
    void* inval_start;
    if (dma_alloc[start_alloc_index].is_mapping)
        inval_start = dma_alloc[start_alloc_index].vaddr +
            ((void*) start - dma_alloc[start_alloc_index].mapped_vaddr);
    else
        inval_start = (void*) start;

    sel4_dma_manager->dma_cache_op_fn(
        sel4_dma_manager->cookie,
        inval_start,
        inval_size,
        DMA_CACHE_OP_INVALIDATE);
}

void sel4_dma_free(void *vaddr)
{
    assert(sel4_dma_manager != NULL);

    // Find the previous allocation.
    int alloc_index = find_allocation_index_by_vaddr(vaddr);
    if (alloc_index < 0) {
        ZF_LOGE("Call to free DMA allocation not in bookkeeping");
        return;
    }

    ZF_LOGD("vaddr = %p, alloc_index = %i", vaddr, alloc_index);

    sel4_dma_manager->dma_free_fn(
        sel4_dma_manager->cookie,
        dma_alloc[alloc_index].vaddr,
        dma_alloc[alloc_index].size);

    // Allocation cleared. Update bookkeeping.
    clear_allocation(alloc_index);
}

void* sel4_dma_memalign(size_t align, size_t size)
{
    assert(sel4_dma_manager != NULL);

    int alloc_index = next_free_allocation_index();
    if (alloc_index < 0) {
        ZF_LOGE("No free DMA allocation slots, unable to allocate");
        return NULL;
    }

    void* vaddr = sel4_dma_manager->dma_alloc_fn(
        sel4_dma_manager->cookie,
        size,
        align,
        false,
        PS_MEM_NORMAL);
    if (vaddr == NULL) {
        ZF_LOGE("DMA allocation returned null pointer");
        return NULL;
    }

    uintptr_t paddr = sel4_dma_manager->dma_pin_fn(
        sel4_dma_manager->cookie,
        vaddr,
        size);
    if (!paddr) {
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
    // Not a mapping.
    dma_alloc[alloc_index].is_mapping = false;
    dma_alloc[alloc_index].mapped_vaddr = NULL;
    dma_alloc[alloc_index].mapping_dir = DMA_NONE;

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
    for (int x = 0; x < MAX_DMA_ALLOCS; x++)
        if (dma_alloc[x].in_use) {
            if (!dma_alloc[x].is_mapping &&
                vaddr >= dma_alloc[x].vaddr &&
                vaddr < dma_alloc[x].vaddr + dma_alloc[x].size)
                return (void*) dma_alloc[x].paddr +
                    (vaddr - dma_alloc[x].vaddr);
            if (dma_alloc[x].is_mapping &&
                vaddr >= dma_alloc[x].mapped_vaddr &&
                vaddr < dma_alloc[x].mapped_vaddr + dma_alloc[x].size)
                return (void*) dma_alloc[x].paddr +
                    (vaddr - dma_alloc[x].mapped_vaddr);
        }

    ZF_LOGE("Unable to determine physical address from virtual %p", vaddr);
    for (int y = 0; y < MAX_DMA_ALLOCS; y++)
        if (dma_alloc[y].in_use)
            ZF_LOGE(" --> Index %i: vaddr = %p, size = 0x%x",
                y, dma_alloc[y].vaddr, dma_alloc[y].size);
    /* This is a fatal error. Not being able to determine an address
        * indicates that we are attempting to communicate with a
        * device via memory that has not been mapped into the physical
        * address space. This implies that additional data needs to be
        * DMA allocated. */
    assert(false);
}

void sel4_dma_initialise(ps_dma_man_t *dma_manager)
{
    sel4_dma_manager = dma_manager;

    for (int x = 0; x < MAX_DMA_ALLOCS; x++)
        clear_allocation(x);
}

void sel4_dma_shutdown(void)
{
    // Deallocate any currently allocated DMA.
    for (int x = 0; x < MAX_DMA_ALLOCS; x++) {
        if (dma_alloc[x].in_use)
            sel4_dma_free(dma_alloc[x].vaddr);
    }

    // Clear the pointer to the DMA routines.
    sel4_dma_manager = NULL;
}

/* Routines to support an implementation of the linux 'DMA mapping' API */

void *sel4_dma_map_single(void* mapped_vaddr, size_t size, enum dma_data_direction dir)
{
    /* Only handle the DMA_TO_DEVICE and DMA_FROM_DEVICE directions */
    if (dir != DMA_TO_DEVICE && dir != DMA_FROM_DEVICE) {
        ZF_LOGE("Unhandled DMA mapping direction: %i", dir);
        return NULL;
    }

    /* Start by creating a DMA allocation */
    void* vaddr = sel4_dma_malloc(size);
    if (vaddr == NULL)
        return NULL;

    /* Now find the index we just allocated to and update the book-keeping
     * with mapping information */
    int alloc_index = find_allocation_index_by_vaddr(vaddr);
    assert(alloc_index >= 0);

    dma_alloc[alloc_index].is_mapping = true;
    dma_alloc[alloc_index].mapped_vaddr = mapped_vaddr;
    dma_alloc[alloc_index].mapping_dir = dir;

    /* Flush the cache to make sure all buffers are aligned */
    sel4_dma_flush_range(mapped_vaddr, mapped_vaddr + size);

    return (void*) dma_alloc[alloc_index].paddr;
}

void sel4_dma_unmap_single(void* paddr)
{
    /* Find the allocation index to be cleared */
    int alloc_index = find_allocation_index_by_paddr((uintptr_t) paddr);
    if (alloc_index < 0) {
        ZF_LOGE("Call to clear DMA mapping not in bookkeeping");
        return;
    }

    if (!dma_alloc[alloc_index].is_mapping) {
        ZF_LOGE("Call to clear DMA mapping not in bookkeeping");
        return;
    }

    void* vaddr = dma_alloc[alloc_index].vaddr;
    void* mapped_vaddr = dma_alloc[alloc_index].mapped_vaddr;
    size_t size = dma_alloc[alloc_index].size;

    /* Flush the cache to make sure all buffers are aligned */
    sel4_dma_flush_range(mapped_vaddr, mapped_vaddr + size);

    /* Now free the DAM allocation (which also clears the mapping) */
    sel4_dma_free(vaddr);
}

/* Map data cache requests on to DMA requests. Note that U-Boot code that is
 * requesting the data cache to be flushed or invalidated is expecting those
 * addresses to be DMA mapped. */

void invalidate_dcache_range(unsigned long start, unsigned long stop)
{
    sel4_dma_invalidate_range((void*) start, (void *) stop);
}

void flush_dcache_range(unsigned long start, unsigned long stop)
{
	sel4_dma_flush_range((void*) start, (void *) stop);
}