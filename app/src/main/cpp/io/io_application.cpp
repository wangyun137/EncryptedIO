/*
* Author: wangyun
* Date: 2017-01-05
* Email: yun.wang@vmplay.com
* LastUpdateTime: 2017-01-05
* LastUpdateBy: wangyun
*/

#include "io_application.h"
#include "io.h"
#include "jni_native.h"

namespace io {

void IOApplication::switchKey(JNIEnv* env, jclass, jstring newKey) {
    const char* newEncryptKey = env->GetStringUTFChars(newKey, 0);
    jstring oldJavaKey = IO::getJavaKey();
    const char* oldEncryptKey = IO::getEncryptKey();
    env->ReleaseStringUTFChars(oldJavaKey, oldEncryptKey);
    IO::setJavaKey(newKey);
    IO::setEncryptKey(newEncryptKey);
}

static JNINativeMethod gApplicationMethod[] = {
    CPP_NATIVE_METHOD(switchKey, "(Ljava/lang/String;)V", IOApplication::switchKey)
};

void IOApplication::registerApplicationMethod(JNIEnv* env) {
    registerNativeMethods(env, "com/singuloid/encrypt/IOApplication",
        gApplicationMethod, sizeof(gApplicationMethod)/sizeof(gApplicationMethod[0]));
}

}
