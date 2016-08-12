/*
 * =====================================================================================
 *
 *       Filename:  compiler.h
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  04/22/2013 10:35:19 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Chen Yuheng (Chen Yuheng), chyh1990@163.com
 *   Organization:  Tsinghua Unv.
 *
 * =====================================================================================
 */

#ifndef __FRAMEWORK_COMPILER_H
#define __FRAMEWORK_COMPILER_H

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>
#include <math.h>

#ifndef _MSC_VER
#include <pthread.h>
#else
/* disable verbose warnings */
#pragma warning(disable: 4244)
#pragma warning(disable: 4305)
#include "stdint_msvc.h"

#ifndef snprintf
#define snprintf _snprintf
#endif

#endif

#undef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif

/* detect platform */
#ifdef __ANDROID__
#define USE_ANDROID
#elif (defined __APPLE__)
	#include "TargetConditionals.h"
	#if ((defined TARGET_OS_IPHONE)  || (defined TARGET_IPHONE_SIMULATOR))
	#define USE_IOS
	#else
	#error "Unsupported platform"
	#endif
#elif defined _WIN32 || defined _WIN64
#define USE_WIN32
#else
/* other */
#endif

#if (defined __SSE2__) || (defined _MSC_VER)
#define USE_SSE2 1
#endif

#if (defined __ARM_NEON__) || (defined __ARM_NEON) || (defined __aarch64__)
#if (defined __arm64__) || (defined __aarch64__)
#define USE_NEON64 1
#else
#define USE_NEON 1
#endif
#endif

/* NDEBUG is defined by build system */
#ifndef NDEBUG
#define FW_DEBUG
#endif

#ifdef FW_DEBUG

#define FW_DEBUG_ON

#define TRACE_BASE_ON
#define ASSERT_BASE_ON
#define WARNING_BASE_ON
#define FIXED_VECTOR_DEBUG
#define TIMESTAMP_BASE_ON

#else

//#define TRACE_BASE_ON
#define WARNING_BASE_ON
//#define ASSERT_BASE_ON
#define TIMESTAMP_BASE_ON
#define EIGEN_NO_DEBUG

#endif

#ifdef _MSC_VER
#define mcv_forceinline __forceinline
#define mcv_ensure_forceinline __forceinline
#else
#define mcv_forceinline __inline__ __attribute__((always_inline))
#define mcv_ensure_forceinline __attribute__((always_inline))
#endif

#ifdef USE_ANDROID

#ifndef LOG_TAG
#define LOG_TAG "imagefw_android"
#endif

#include <android/log.h>
#define  __ANDROID_LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  __ANDROID_LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define  __TRACE_OUTPUT(...)  __ANDROID_LOGI(__VA_ARGS__)


#include "android_runtime/AndroidRuntime.h"


#endif /* android */

/* msvc sucks, again */
#if defined(_MSC_VER)
#define __PACK_STRUCT
#else
#define __PACK_STRUCT __attribute__((packed))
#endif

#define STATIC_ASSERT( condition, name )\
	    typedef char assert_failed_ ## name [ (condition) ? 1 : -1 ];

#ifndef __TRACE_OUTPUT
#define __TRACE_OUTPUT(...) do{fprintf(stderr, __VA_ARGS__);}while(0)
#endif

#ifndef _MSC_VER

#ifdef TRACE_BASE_ON
#define FW_DEBUG_TRACE(fmt, args...) __TRACE_OUTPUT("TRACE [%s:%d]: " fmt "\n",\
	__FILE__, __LINE__, ##args)
#else
#define FW_DEBUG_TRACE(args ...) do{}while(0)
#endif

#ifdef WARNING_BASE_ON
#define FW_DEBUG_WARNING(fmt, args...) __TRACE_OUTPUT("WARNING [%s:%d]: " fmt "\n", \
	__FILE__, __LINE__, ##args)
#else
#define FW_DEBUG_WARNING(args ...) do{}while(0)
#endif
#else
/* TODO */
#define FW_DEBUG_TRACE(...) do{}while(0)
#define FW_DEBUG_WARNING( ...) do{}while(0)

#endif

#define FW_CHECK(cond) do{if(!(cond)) {FW_DEBUG_WARNING("Assert failed: %s", #cond); \
	assert(cond);}}while(0)

#ifdef ASSERT_BASE_ON
#define FW_ASSERT(cond) FW_CHECK(cond)
#else
#define FW_ASSERT(cond) do{}while(0)
#endif

/* always output */
#define FW_PRINTF(...) __TRACE_OUTPUT(__VA_ARGS__)

#ifdef USE_NEON
#define FW_SIMD_ALIGN_SHIFT 0
#elif (defined USE_SSE2)
#define FW_SIMD_ALIGN_SHIFT 4
#else
#define FW_SIMD_ALIGN_SHIFT 4
#endif

#define FW_SIMD_ALIGN (1<<FW_SIMD_ALIGN_SHIFT)
#define FW_PTR_ALIGNED(ptr, n) (((size_t)ptr & (n-1)) == 0)
#define FW_PTR_SIMD_ALIGNED(ptr) FW_PTR_ALIGNED(ptr, FW_SIMD_ALIGN)

#define FW_NOT_IMPLEMENTED do{FW_PRINTF(__FUNCTION__ " not implemented\n");}while(0)

#ifdef __cplusplus
#ifndef DISABLE_COPY_AND_ASSIGN
#define DISABLE_COPY_AND_ASSIGN(classname) \
private:\
  classname(const classname&);\
  classname& operator=(const classname&)
#endif
#endif

#ifndef _MSC_VER
#define MCV_ENSURE_CONSTRUCTOR(name) \
	extern "C" void name(void); \
	void (*___e_##name)(void) = name;
#else
#define MCV_ENSURE_CONSTRUCTOR(name)
#endif

#ifdef __cplusplus
extern "C"{
#endif

void *xmalloc_align16(size_t size);
void xfree_align16(void *p);

#ifdef __cplusplus
}
#endif

#endif

