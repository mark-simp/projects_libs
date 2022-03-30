void sel4_dma_flush_range(unsigned long start, unsigned long stop);

void sel4_dma_invalidate_range(unsigned long start, unsigned long stop);

void sel4_dma_free(void *vaddr);

void* sel4_dma_memalign(size_t align, size_t size);

void* sel4_dma_malloc(size_t size);

void* sel4_dma_virt_to_phys(void *vaddr);

void* sel4_dma_phys_to_virt(void *paddr);

bool sel4_dma_is_virt_mapped(void * vaddr);