/* Wrappers around U-Boot print string routines */

#include <utils/zf_log.h>

#define puts(...)           ZF_LOGD(__VA_ARGS__)

#define debug(...)          ZF_LOGD(__VA_ARGS__)

#define pr_debug(...)       ZF_LOGD(__VA_ARGS__)

#define log(A, B, ...)      ZF_LOGD(__VA_ARGS__)

#define log_debug(...)      ZF_LOGD(__VA_ARGS__)

#define log_warning(...)    ZF_LOGW(__VA_ARGS__)

#define printf(...)         ZF_LOGD(__VA_ARGS__)

#define log_ret(_ret) ({ \
	int __ret = (_ret); \
	if (__ret < 0) \
		ZF_LOGE("returning err=%d\n", __ret); \
	__ret; \
	})

#define log_msg_ret(_msg, _ret) ({ \
	int __ret = (_ret); \
	if (__ret < 0) \
		ZF_LOGE("%s: returning err=%d\n", _msg, __ret); \
	__ret; \
	})
