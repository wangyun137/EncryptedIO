/*
* Author: wangyun
* Date: 2017-01-05
* Email: yun.wang@vmplay.com
* LastUpdateTime: 2017-01-05
* LastUpdateBy: wangyun
*/

#ifndef  ENCRYPTED_CRYPTO_HELPER_H
#define ENCRYPTED_CRYPTO_HELPER_H

#include <jni.h>
#include <string>

using std::string;

namespace io {

typedef struct DataHead {
    uint32_t magic;
    uint32_t clear_size;
    uint32_t cipher_size;
} DataHead;

class CryptoHelper {
public:
    static void initDataHead(DataHead* head);

    static DataHead readDataHead(const char* buf, int len);

    static bool isEncrypted(DataHead* head);

    static jbyteArray encryptData(JNIEnv* env, jclass, jbyteArray javaBytes, jint javaLen);

    static jbyteArray decryptData(JNIEnv* env, jclass, jbyteArray javaBytes, jint javaLen);

    static void encryptFile(JNIEnv* env, jclass clazz, jstring javaSrcPath, jstring javaDestPath);

    static void encryptFd(JNIEnv* env, jclass, jint javaFd, jstring javaDestPath);

    static jint decryptFile(JNIEnv* env, jclass clazz, jstring javaSrcPath, jstring javaDestPath);

    static jint decryptFd(JNIEnv* env, jclass, jint javaFd, jstring javaDestPath);

    static void registerCryptoHelperMethods(JNIEnv* env);

}; // class CryptoHelper

} // namespace io

#endif // ENCRYPTED_CRYPTO_HELPER_H
