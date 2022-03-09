#include <uboot_helper.h>

unsigned long get_timer(unsigned long base) {
    return 0;
}

unsigned long  timer_get_us(void) {
    return 0;
}

void flush_dcache_range(unsigned long start, unsigned long stop) { }

void invalidate_dcache_range(unsigned long start, unsigned long stop) { }

int console_assign(int file, const char *devname) { }