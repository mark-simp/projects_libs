/* Wrappers around U-Boot print string routines */

#include <utils/zf_log.h>

#define puts(...)   ZF_LOGD(__VA_ARGS__)

#define debug(...)  ZF_LOGD(__VA_ARGS__)

#define printf(...)  ZF_LOGD(__VA_ARGS__)
