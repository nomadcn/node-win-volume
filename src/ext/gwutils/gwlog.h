#ifndef __GW_LOG_H__
#define __GW_LOG_H__

#include <string.h>
#include "Logger.h"

#define GW_LOG_VERBOSE	1
#define GW_LOG_DEBUG	1
#define GW_LOG_INFO	    1
#define GW_LOG_WARN	    1
#define GW_LOG_ERROR	1

#define __FILENAME__	((strrchr(__FILE__, CH_PATH_DELIMITER) == NULL) ? __FILE__ : strrchr(__FILE__, CH_PATH_DELIMITER) + 1)

#if GW_LOG_VERBOSE
#define LOGV(FILTER, ...) \
	GetLogger().Print(eLL_VERBOSE, FILTER, __VA_ARGS__);
#define LOGV_CALL(FILTER, MSG) \
	GetLogger().Print(eLL_VERBOSE, FILTER, "%s(%d) %s: %s", __FILENAME__, __LINE__, __FUNCTION__, MSG);
#define LOGV_LINE(FILTER) \
	GetLogger().Print(eLL_VERBOSE, FILTER, "%s(%d) %s", __FILENAME__, __LINE__, __FUNCTION__);
#else
#define LOGV(FILTER, ...) 0
#define LOGV_CALL(FILTER, MSG) 0
#define LOGV_LINE(FILTER) 0
#endif

#if GW_LOG_DEBUG
#define LOGD(FILTER, ...) \
	GetLogger().Print(eLL_DEBUG, FILTER, __VA_ARGS__);
#define LOGD_CALL(FILTER, MSG) \
	GetLogger().Print(eLL_DEBUG, FILTER, "%s(%d) %s: %s", __FILENAME__, __LINE__, __FUNCTION__, MSG);
#define LOGD_LINE(FILTER) \
	GetLogger().Print(eLL_DEBUG, FILTER, "%s(%d) %s", __FILENAME__, __LINE__, __FUNCTION__);
#else
#define LOGD(FILTER, ...) 0
#define LOGD_CALL(FILTER, MSG) 0
#define LOGD_LINE(FILTER) 0
#endif

#if GW_LOG_INFO
#define LOGI(FILTER, ...) \
	GetLogger().Print(eLL_INFO, FILTER, __VA_ARGS__);
#define LOGI_CALL(FILTER, MSG) \
	GetLogger().Print(eLL_INFO, FILTER, "%s(%d) %s: %s", __FILENAME__, __LINE__, __FUNCTION__, MSG);
#define LOGI_LINE(FILTER) \
	GetLogger().Print(eLL_INFO, FILTER, "%s(%d) %s", __FILENAME__, __LINE__, __FUNCTION__);
#else
#define LOGI(FILTER, ...) 0
#define LOGI_CALL(FILTER, MSG) 0
#define LOGI_LINE(FILTER) 0
#endif

#if GW_LOG_WARN
#define LOGW(FILTER, ...) \
	GetLogger().Print(eLL_WARN, FILTER, __VA_ARGS__);
#define LOGW_CALL(FILTER, MSG) \
	GetLogger().Print(eLL_WARN, FILTER, "%s(%d) %s: %s", __FILENAME__, __LINE__, __FUNCTION__, MSG);
#define LOGW_LINE(FILTER) \
	GetLogger().Print(eLL_WARN, FILTER, "%s(%d) %s", __FILENAME__, __LINE__, __FUNCTION__);
#else
#define LOGW(FILTER, ...) 0
#define LOGW_CALL(FILTER, MSG) 0
#define LOGW_LINE(FILTER) 0
#endif

#if GW_LOG_ERROR
#define LOGE(FILTER, ...) \
	GetLogger().Print(eLL_ERROR, FILTER, __VA_ARGS__);
#define LOGE_CALL(FILTER, MSG) \
	GetLogger().Print(eLL_ERROR, FILTER, "%s(%d) %s: %s", __FILENAME__, __LINE__, __FUNCTION__, MSG);
#define LOGE_LINE(FILTER) \
	GetLogger().Print(eLL_ERROR, FILTER, "%s(%d) %s", __FILENAME__, __LINE__, __FUNCTION__);

#if !defined(ASSERT)
#define ASSERT(x) \
if(!(x)) \
{ \
	GetLogger().Print(eLL_ERROR, "ERROR", "%s(%d) %s", __FILENAME__, __LINE__, __FUNCTION__); \
}
#endif
#else
#define LOGE(FILTER, ...) 0
#define LOGE_CALL(FILTER, MSG) 0
#define LOGE_LINE(FILTER) 0
#define ASSERT(x) 0
#endif

#define __CALL_INFO__ __FILENAME__, __LINE__, __FUNCTION__

#endif // __GW_LOG_H__
