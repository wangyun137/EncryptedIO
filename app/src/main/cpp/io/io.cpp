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

namespace io {

const char* IO::gEncryptKey_ = NULL;

void IO::onLoad(JavaVM* vm, JNIEnv* env, void* reserved) {

}

void IO::setEncryptKey(const char* key) {
    gEncryptKey_ = key;
}

const char* IO::getEncryptKey() {
    return gEncryptKey_;
}

} // end namespace io


jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    JNIEnv* env = NULL;
    jint version = vm->GetEnv((void **)&env, JNI_VERSION_1_6);

    //TODO:find encrypt key and set it

    io::IO* io = new io::IO;
    io->onLoad(vm, env, reserved);
    delete io;

    return version;
}
