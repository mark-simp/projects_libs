#include <uboot_helper.h>

unsigned long get_timer(unsigned long base) {
    return 0;
}

unsigned long  timer_get_us(void) {
    return 0;
}

int console_assign(int file, const char *devname) { }

/* Not required / used routines. Ensure they are not used by raising an assert */
long get_ram_size(long *base, long size) {
    assert(false);
}

void *locate_dtb_in_fit(const void *fit) {
    assert(false);
}

u32 dm_pci_read_bar32(const struct udevice *dev, int barnum) {
    assert(false);
}
