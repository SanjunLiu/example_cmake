/*
 * apicommon.h
 *
 *  Created on: May 23, 2016
 *      Author: ubuntu
 */

#ifndef APICOMMON_H_
#define APICOMMON_H_

#ifdef TEST_BUILD_ANDROID

#ifdef _MSC_VER
#   ifdef __cplusplus
#       ifdef CV_STATIC_LIB
#           define CV_SDK_API  extern "C"
#       else
#           ifdef SDK_EXPORTS
#               define CV_SDK_API extern "C" __declspec(dllexport)
#           else
#               define CV_SDK_API extern "C" __declspec(dllimport)
#           endif
#       endif
#   else
#       ifdef CV_STATIC_LIB
#           define CV_SDK_API
#       else
#           ifdef SDK_EXPORTS
#               define CV_SDK_API __declspec(dllexport)
#           else
#               define CV_SDK_API __declspec(dllimport)
#           endif
#       endif
#   endif
#else // _MSC_VER
#   ifdef __cplusplus
#       ifdef SDK_EXPORTS
#           define CV_SDK_API  extern "C" __attribute__((visibility ("default")))
#       else
#           define CV_SDK_API extern "C"
#       endif
#   else
#       ifdef SDK_EXPORTS
#           define CV_SDK_API __attribute__((visibility ("default")))
#       else
#           define CV_SDK_API
#       endif
#   endif
#endif // _MSC_VER


#include <time.h>

#include <android/log.h>
#define LOG_TAG "JNI"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

#endif //TEST_BUILD_ANDROID


#endif // APICOMMON_H_
