#include <uboot_helper.h>
#include <dm/device.h>

/* This is required to prevent a linker error but is never used (because OF_PLATDATA_INST is not defined) */
struct udevice _u_boot_udevice__root;

int console_assign(int file, const char *devname) { }

/* Not required routine. Perform a 'nop' */
void invalidate_dcache_range(unsigned long start, unsigned long stop)
{
	// Not required. seL4 DMA cache operations will be used instead for any DMA memory.
}

void flush_dcache_range(unsigned long start, unsigned long stop)
{
	// Not required. seL4 DMA cache operations will be used instead for any DMA memory.
}

/* Unused routines. Ensure they are not used by raising an assert */
long get_ram_size(long *base, long size) {
    assert(false);
}

void *locate_dtb_in_fit(const void *fit) {
    assert(false);
}

u32 dm_pci_read_bar32(const struct udevice *dev, int barnum) {
    assert(false);
}
