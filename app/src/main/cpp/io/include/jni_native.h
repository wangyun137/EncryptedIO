/*
* Author: wangyun
* Date: 2016-12-30
* Email: yun.wang@vmplay.com
* LastUpdateTime: 2017-01-03
* LastUpdateBy: wangyun
*/
#ifndef ENCRYPTED_JNI_NATIVE_H
#define ENCRYPTED_JNI_NATIVE_H

#include <jni.h>
#include <stdlib.h>
#include <sys/system_properties.h>

#define GET_SDK_VERSION(versionChar, versionInt) \
    __system_property_get("ro.build.version.sdk", versionChar); \
    versionInt = atoi(versionChar); \

#define CPP_NATIVE_METHOD(functionName, signature, address) \
  { #functionName, signature, reinterpret_cast<void *>(address) }

extern int registerNativeMethods(JNIEnv* env, const char* className,
      const JNINativeMethod* methods, int size);

#endif
