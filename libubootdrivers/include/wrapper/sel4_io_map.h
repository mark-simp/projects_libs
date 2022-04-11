#include <sel4platsupport/io.h>

void sel4_io_map_initialise(ps_io_mapper_t *io_mapper);

void sel4_io_map_shutdown(void);

void sel4_io_map_do_unmap(void *vaddr);

void* sel4_io_map_do_map(uintptr_t paddr, size_t size);

void* sel4_io_map_virt_to_phys(void *vaddr);

void* sel4_io_map_phys_to_virt(void *paddr);

bool sel4_io_map_is_mapped(void *vaddr);