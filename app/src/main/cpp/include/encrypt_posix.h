/*
* Author: wangyun
* Date: 2016-12-30
* Email: yun.wang@vmplay.com
* LastUpdateTime: 2017-01-03
* LastUpdateBy: wangyun
*/
#ifndef ENCRYPTED_ENCRYPT_POSIX_H
#define ENCRYPTED_ENCRYPT_POSIX_H

#include <jni.h>

namespace io {

typedef struct JniConstants {
    static void init(JNIEnv* env);
    static jclass errnoExceptionClass;
    static jclass mutableIntClass;
    static jclass structStatClass;
} JniConstants;

class EncryptPosix {

public:
    static jobject open(JNIEnv* env, jobject, jstring javaPath, jint javaFlags, jint mode);

    static void close(JNIEnv* env, jobject, jobject javaFd);

    static jobject stat(JNIEnv* env, jobject, jstring javaPath);

    static jobject fstat(JNIEnv* env, jobject, jobject javaFd);

    static jint preadBytes(JNIEnv* env, jobject, jobject javaFd, jobject javaBytes,
          jint javaBytesOffset, jint javaByteCount, jlong javaOffset);

    static jint pwriteBytes(JNIEnv* env, jobject, jobject javaFd, jbyteArray javaBytes,
          jint javaBytesOffset, jint javaByteCount, jlong javaOffset);

    static jint readBytes(JNIEnv* env, jobject, jobject javaFd, jobject javaBytes,
          jint javaBytesOffset, jint javaBytesCount);

    static jint writeBytes(JNIEnv* env, jobject, jobject javaFd, jbyteArray javaBytes,
          jint javaBytesOffset, jint javaBytesCount);

    static jlong mmap(JNIEnv* env, jobject, jlong javaAddress, jlong javaBytesCount,
          jint javaProt, jint javaFlags, jobject javaFd, jlong javaOffset);

    static void ftruncate(JNIEnv* env, jobject, jobject javaFd, jlong javaLen);

    static long lseek(JNIEnv* env, jobject, jobject javaFd, jlong javaOffset, jint javaWhence);

}; // class EncryptPosix


} // namespace io

#endif //ENCRYPTED_ENCRYPT_POSIX_H
