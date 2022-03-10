#include <stddef.h>
#include <linux/types.h>

/*
 * No environment variable support. Return NULL / zero.
 */
char *env_get(const char *varname) {
    return NULL;
}

ulong env_get_hex(const char *varname, ulong default_val) {
    return 0;
}
