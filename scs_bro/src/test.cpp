#include "apicommon.h"

#ifdef TEST_BUILD_ANDROID

CV_SDK_API
void printHello(){

    LOGI("Hello Android");
}

#endif
