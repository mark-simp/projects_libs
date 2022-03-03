#include <platsupport/delay.h>

void udelay(unsigned long usec);
{
    ps_udelay(usec);
}

void __udelay(unsigned long usec);
{
    ps_udelay(usec);
}
