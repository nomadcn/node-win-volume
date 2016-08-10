#ifndef GW_H
#define GW_H

#include <stdint.h>

#if defined(WIN32) || defined(WIN64)
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include <crtdbg.h>
#include <tchar.h>
#endif // #ifdef WIN32

/**
 * This is the base header for all other gw series.
 * DON'T include class and function header files here.
 */

#if defined(__cplusplus)

#ifndef GW_NAMESPACE_NAME
#define GW_NAMESPACE_NAME gw
#define GW_NAMESPACE_BEGIN namespace GW_NAMESPACE_NAME {
#define GW_NAMESPACE_END }
#define GW_NAMESPACE_USE using namespace GW_NAMESPACE_NAME;
#define USE_GW_NAMESPACE using namespace GW_NAMESPACE_NAME;
#endif

// extern c
#if !defined(EXTERN_C_BEGIN)
#define EXTERN_C_BEGIN extern "C" {
#define EXTERN_C_END }
#endif

#else

#define GW_NAMESPACE_NAME
#define GW_NAMESPACE_BEGIN
#define GW_NAMESPACE_END
#define GW_NAMESPACE_USE
#define USE_GW_NAMESPACE
#define EXTERN_C_BEGIN
#define EXTERN_C_END

#endif // __cplusplus

// 64bit environment macro
#if _WIN32 || _WIN64
	#if _WIN64
		#define ENV64BIT
	#else
		#define ENV32BIT
	#endif
#elif __GNUC__
	#if __x86_64__ || __ppc64__
		#define ENV64BIT
	#else
		#define ENV32BIT
	#endif
#endif

GW_NAMESPACE_BEGIN

//#include "gwerror.h"
#include "gwport.h"

GW_NAMESPACE_END

#endif
