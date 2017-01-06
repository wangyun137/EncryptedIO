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
#include <string>

using std::string;

namespace io {

class IO {

public:
    IO(){}

    ~IO(){}

    void onLoad(JNIEnv* env);

    static string getEncryptKey();

    static void setEncryptKey(string key);

private:
    static string gEncryptKey_;
};// class io

} // namespace io

#endif // end of ENCRYPTED_IO_H
