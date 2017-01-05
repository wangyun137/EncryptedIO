/*
* Author: wangyun
* Date: 2017-01-04
* Email: yun.wang@vmplay.com
* LastUpdateTime: 2017-01-04
* LastUpdateBy: wangyun
*/
#include "jni_native.h"
#include "scoped_local_ref.h"

#include <stdio.h>

int registerNativeMethods(JNIEnv* env, const char* className,
      const JNINativeMethod* methods, int size) {

    ScopedLocalRef<jclass> registerClass(env, env->FindClass(className));
    if (registerClass.get() == NULL) {
        char* tmp;
        const char* msg;
        if (asprintf(&tmp, "Native registration unable to find class '%s'; aborting...",
                     className) == -1) {
            msg = "Native registration unable to find class; aborting...";
        } else {
            msg = tmp;
        }
        env->FatalError(msg);
    }
    if (env->RegisterNatives(registerClass.get(), methods, size) < 0) {
        char* tmp;
        const char* msg;
        if (asprintf(&tmp, "RegisterNatives failed for '%s'; aborting...", className) == -1) {
            // Allocation failed, print default warning.
            msg = "RegisterNatives failed; aborting...";
        } else {
            msg = tmp;
        }
        env->FatalError(msg);
    }
    return 0;
}
