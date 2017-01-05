/*
* Author: wangyun
* Date: 2017-01-04
* Email: yun.wang@vmplay.com
* LastUpdateTime: 2017-01-04
* LastUpdateBy: wangyun
*/

#include "io.h"
#include "encrypt_posix.h"
#include "jni_native.h"
#include "io_application.h"
#include "crypto_helper.h"

using io::EncryptPosix;

static JNINativeMethod gPosixMethods[] = {
    CPP_NATIVE_METHOD(open,       "(Ljava/lang/String;II)Ljava/io/FileDescriptor;",        EncryptPosix::open),
    CPP_NATIVE_METHOD(close,      "(Ljava/io/FileDescriptor;)V",                           EncryptPosix::close),
    CPP_NATIVE_METHOD(stat,       "(Ljava/lang/String;)Landroid/system/StructStat;",       EncryptPosix::stat),
    CPP_NATIVE_METHOD(fstat,      "(Ljava/io/FileDescriptor;)Landroid/system/StructStat;", EncryptPosix::fstat),
    CPP_NATIVE_METHOD(preadBytes, "(Ljava/io/FileDescriptor;Ljava/lang/Object;IIJ)I",      EncryptPosix::preadBytes),
    CPP_NATIVE_METHOD(pwriteBytes,"(Ljava/io/FileDescriptor;Ljava/lang/Object;IIJ)I",      EncryptPosix::pwriteBytes),
    CPP_NATIVE_METHOD(readBytes,  "(Ljava/io/FileDescriptor;Ljava/lang/Object;II)I",       EncryptPosix::readBytes),
    CPP_NATIVE_METHOD(writeBytes, "(Ljava/io/FileDescriptor;Ljava/lang/Object;II)I",       EncryptPosix::writeBytes),
    CPP_NATIVE_METHOD(mmap,       "(JJIILjava/io/FileDescriptor;J)J",                      EncryptPosix::mmap),
    CPP_NATIVE_METHOD(ftruncate,  "(Ljava/io/FileDescriptor;J)V",                          EncryptPosix::ftruncate),
    CPP_NATIVE_METHOD(lseek,      "(Ljava/io/FileDescriptor;JI)J",                         EncryptPosix::lseek)
};

static void registerPosixMethods(JNIEnv* env) {
    char sdkVersion[10];
    int version;
    GET_SDK_VERSION(sdkVersion, version);

    if (version >= 21) {
        registerNativeMethods(env, "libcore/io/Posix",
            gPosixMethods, sizeof(gPosixMethods) / sizeof(gPosixMethods[0]));
    }
}

namespace io {

const char* IO::gEncryptKey_ = NULL;

void IO::onLoad(JNIEnv* env) {
    registerPosixMethods(env);
    IOApplication::registerApplicationMethod(env);
    CryptoHelper::registerCryptoHelperMethods(env);
}

void IO::setEncryptKey(const char* key) {
    gEncryptKey_ = key;
}

const char* IO::getEncryptKey() {
    return gEncryptKey_;
}

jstring IO::getJavaKey() {
    return gJavaKey_;
}

void IO::setJavaKey(jstring key) {
    gJavaKey_ = key;
}

} // end namespace io


jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    JNIEnv* env = NULL;
    jint version = vm->GetEnv((void **)&env, JNI_VERSION_1_6);

    jclass appClass = env->FindClass("com.singuloid.encrypt.IOApplication");
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        env->ThrowNew(env->FindClass("java/lang/ClassNotFoundException"),
            "can not find com.singuloid.encrypt.IOApplication");
    }
    static jfieldID keyId = env->GetStaticFieldID(appClass, "sEncryptKey", "Ljava/lang/String;");
    jstring javaKey = (jstring)env->GetStaticObjectField(appClass, keyId);
    //TODO:when release the resource
    const char* encryptKey = env->GetStringUTFChars(javaKey, 0);

    io::IO::setJavaKey(javaKey);
    io::IO::setEncryptKey(encryptKey);

    io::IO* io = new io::IO;
    io->onLoad(env);
    delete io;

    return version;
}
