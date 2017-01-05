/*
* Author: wangyun
* Date: 2017-01-04
* Email: yun.wang@vmplay.com
* LastUpdateTime: 2017-01-04
* LastUpdateBy: wangyun
*/
#ifndef ENCRYPTED_IO_H
#define ENCRYPTED_IO_H

#include <jni.h>

namespace io {

class IO {

public:
    IO(){}

    ~IO(){}

    void onLoad(JNIEnv* env);

    static const char* getEncryptKey();

    static void setEncryptKey(const char* key);

    static jstring getJavaKey();

    static void setJavaKey(jstring key);

private:
    static const char* gEncryptKey_;

    static jstring gJavaKey_;
};// class io

} // namespace io

#endif // end of ENCRYPTED_IO_H
