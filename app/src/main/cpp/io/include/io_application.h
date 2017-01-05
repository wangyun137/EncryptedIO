/*
* Author: wangyun
* Date: 2017-01-05
* Email: yun.wang@vmplay.com
* LastUpdateTime: 2017-01-05
* LastUpdateBy: wangyun
*/

#ifndef ENCRYPTED_IO_APPLICATION_H
#define ENCRYPTED_IO_APPLICATION_H

#include <jni.h>

namespace io {

class IOApplication {

public:
    static void switchKey(JNIEnv* env, jclass, jstring newKey);

    static void registerApplicationMethod(JNIEnv* env);
};

} // namespace io

#endif //ENCRYPTED_IO_APPLICATION_H
