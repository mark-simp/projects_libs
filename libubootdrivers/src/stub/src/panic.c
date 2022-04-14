/* Provide an seL4 compliant implemention of panic as used by U-Boot */

#include <uboot_helper.h>
#include <hang.h>

void hang(void)
{
	assert(false);
}

void panic(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	printf(fmt, args);
	va_end(args);
	hang();
}