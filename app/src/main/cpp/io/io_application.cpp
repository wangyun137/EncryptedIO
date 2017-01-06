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

#include <string>

using std::string;

namespace io {

void IOApplication::switchKey(JNIEnv* env, jclass, jstring newKey) {
    const char* newEncryptKeyChar = env->GetStringUTFChars(newKey, 0);
    string newEncryptKey = string(newEncryptKey);
    env->ReleaseStringUTFChars(newKey, newEncryptKeyChar);
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
