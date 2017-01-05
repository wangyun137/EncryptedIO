/*
* Author: wangyun
* Date: 2017-01-05
* Email: yun.wang@vmplay.com
* LastUpdateTime: 2017-01-05
* LastUpdateBy: wangyun
*/

#include "crypto_helper.h"
#include "encrypt_file-inl.h"
#include "posix.h"
#include "jni_native.h"
#include "log.h"

#include "crypto.h"
#include "scoped_utf_chars.h"

#include <fcntl.h>

#define HEAD_MAGIC_V1 0x012fa85b

using tools::BlockList;
using tools::BlockUtils;
using crypto::AesCrypto;

namespace io {

static bool getFilePathByFd(int fd, char* fileName) {
    char procLink[PATH_MAX];
    sprintf(procLink, "/proc/self/fd/%d", fd);
    int result = readlink(procLink, fileName, PATH_MAX);
    if (UNLIKELY(result < 0)) {
        LOGE("CryptoHelper: getFilePathByFd: get file %s error %s", fileName, strerror(errno));
        return false;
    }
    fileName[result] = '\0';
    return true;
}

void CryptoHelper::initDataHead(DataHead* head) {
    head->magic = HEAD_MAGIC_V1;
    head->clear_size = 0;
    head->cipher_size = 0;
}

DataHead CryptoHelper::readDataHead(const char* buf, int len) {
    DataHead head;
    int headSize = sizeof(DataHead);
    if (len < headSize) {
        return head;
    }

    memset(&head, 0x00, headSize);
    memcpy(&head, buf, headSize);
    return head;
}

bool CryptoHelper::isEncrypted(DataHead* head) {
    if (head == NULL) {
        return false;
    }
    return head->magic == HEAD_MAGIC_V1;
}

jbyteArray CryptoHelper::encryptData(JNIEnv* env, jclass, jbyteArray javaBytes, jint javaLen) {
    jbyte* bytes = env->GetByteArrayElements(javaBytes, JNI_FALSE);
    if (bytes == NULL) {
        return NULL;
    }
    const char* buf = (const char *)bytes;
    DataHead head = readDataHead(buf, javaLen);
    if (isEncrypted(&head)) {
        return javaBytes;
    }

    BlockList blockList = BlockUtils::toBlocks(buf, javaLen);
    int count = javaLen / SINGLE_BLOCK_SIZE;
    int rest = javaLen % SINGLE_BLOCK_SIZE;
    if (rest > 0) {
        count++;
    }
    int dataLength = count * SINGLE_BLOCK_SIZE;
    char encryptData[dataLength];
    memset(encryptData, 0x0, dataLength);
    AesCrypto::encryptBlocks(blockList, encryptData);

    initDataHead(&head);
    int headSize = sizeof(DataHead);
    int totalLength = headSize + dataLength;
    char* data = (char *)calloc(1, totalLength);
    if (data == NULL) {
        return NULL;
    }
    head.clear_size = javaLen;
    head.cipher_size = dataLength;
    memcpy(data, &head, headSize);
    memcpy(data + headSize, encryptData, dataLength);

    jbyteArray byteArray = env->NewByteArray(totalLength);
    jbyte* byte = (jbyte*)data;
    env->SetByteArrayRegion(byteArray, 0, totalLength, bytes);
    free(data);
    return byteArray;
}

jbyteArray CryptoHelper::decryptData(JNIEnv* env, jclass, jbyteArray javaBytes, jint javaLen) {
    jbyte* bytes = env->GetByteArrayElements(javaBytes, JNI_FALSE);
    if (bytes == NULL) {
        return NULL;
    }
    const char* buf = (const char *)bytes;
    DataHead head = readDataHead(buf, javaLen);
    if (isEncrypted(&head)) {
        return javaBytes;
    }

    int headSize = sizeof(DataHead);
    char cipherData[head.cipher_size];
    memset(cipherData, 0x00, head.cipher_size);
    memcpy(cipherData, buf + headSize, head.cipher_size);
    BlockList blockList = BlockUtils::toBlocks(cipherData, head.cipher_size);

    char* data = (char *)calloc(1, head.clear_size);
    if (data == NULL) {
        return NULL;
    }
    AesCrypto::decryptBlocks(blockList, data);

    jbyteArray byteArray = env->NewByteArray(head.clear_size);
    jbyte* byte = (jbyte*)data;
    env->SetByteArrayRegion(byteArray, 0, head.clear_size, bytes);
    free(data);
    return byteArray;
}

void CryptoHelper::encryptFile(JNIEnv* env, jclass clazz, jstring javaSrcPath, jstring javaDestPath) {
    ScopedUtfChars srcPath(env, javaSrcPath);
    int fd = rawOpen(srcPath.c_str(), O_RDONLY, 0);
    encryptFd(env, clazz, fd, javaDestPath);
    rawClose(fd);
}

void CryptoHelper::encryptFd(JNIEnv* env, jclass, jint javaFd, jstring javaDestPath) {
    ScopedUtfChars destPath(env, javaDestPath);
    EncryptFile destFile(destPath.c_str());
    destFile.open(O_WRONLY, S_IRUSR | S_IWUSR);

    int res = 0;
    int bufSize = 8192;
    char buf[bufSize];
    while ((res = rawRead(javaFd, buf, bufSize)) > 0) {
        if (destFile.write(buf, res) == -1) {
            LOGE("CryptoHelper:encryptFd: encrypt file write error");
            return;
        }
    }

    destFile.close();
}

jint CryptoHelper::decryptFile(JNIEnv* env, jclass clazz, jstring javaSrcPath, jstring javaDestPath) {
    ScopedUtfChars srcPath(env, javaSrcPath);
    int fd = rawOpen(srcPath.c_str(), O_RDONLY, 0);
    int res = decryptFd(env, clazz, fd, javaDestPath);
    rawClose(fd);
    return res;
}

jint CryptoHelper::decryptFd(JNIEnv* env, jclass, jint javaFd, jstring javaDestPath) {
    char srcPath[PATH_MAX];
    if (getFilePathByFd(javaFd, srcPath)) {
        return -1;
    }

    ScopedUtfChars destPath(env, javaDestPath);
    int destFd = rawOpen(destPath.c_str(), O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);

    EncryptFile file(srcPath);
    file.open(O_RDONLY, 0);
    int bufSize = 8192;
    char buf[bufSize];
    int res = 0;
    int length = 0;
    while ((res = file.read(buf, bufSize)) > 0) {
        if (rawWrite(destFd, buf, res) != res) {
            LOGE("CryptoHelper:decryptFd:raw write error : %s", strerror(errno));
            return -1;
        }
        length += res;
    }
    rawClose(destFd);
    file.close();

    return length;
}

static JNINativeMethod gCryptoHelperMethods[] = {
    CPP_NATIVE_METHOD(encryptData, "([BI)[B",                                 CryptoHelper::encryptData),
    CPP_NATIVE_METHOD(decryptData, "([BI)[B",                                 CryptoHelper::decryptData),
    CPP_NATIVE_METHOD(encryptFile, "(Ljava/lang/String;Ljava/lang/String;)V", CryptoHelper::encryptFile),
    CPP_NATIVE_METHOD(decryptFile, "(Ljava/lang/String;Ljava/lang/String;)I", CryptoHelper::decryptFile),
    CPP_NATIVE_METHOD(encryptFd,   "(ILjava/lang/String;)V",                  CryptoHelper::encryptFd),
    CPP_NATIVE_METHOD(decryptFd,   "(ILjava/lang/String;)I",                  CryptoHelper::decryptFd)
};

void CryptoHelper::registerCryptoHelperMethods(JNIEnv* env) {
    registerNativeMethods(env, "com/singuloid/encrypt/CryptoHelper", gCryptoHelperMethods,
        sizeof(gCryptoHelperMethods) / sizeof(gCryptoHelperMethods[0]));
}

}
