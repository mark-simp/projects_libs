/* Provide an seL4 compliant implemention of panic as used by U-Boot */

#include <stdio.h>
#include <stdarg.h>
#include <hang.h>

void panic(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
	hang();
}