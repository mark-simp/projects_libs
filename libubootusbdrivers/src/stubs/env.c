#include <stddef.h>

/*
 * No environment variable support. Return NULL.
 */
char *env_get(const char *varname) {
    return NULL;
}
