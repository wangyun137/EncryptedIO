/*
* Author: wangyun
* Date: 2016-12-30
* Email: yun.wang@vmplay.com
* LastUpdateTime: 2017-01-03
* LastUpdateBy: wangyun
*/
#include "encrypt_posix.h"
#include "encrypt_file-inl.h"
#include "posix.h"
#include "macros.h"

#include "jni/jni_help.h"
#include "jni/jni_constants.h"
#include "jni/scoped_local_ref.h"
#include "jni/scoped_bytes.h"
#include "jni/scoped_utf_chars.h"


#include <list>
#include <string>


using std::string;
using std::list;

namespace io {

class EncryptFile;

/*
* declare some useful internal methods
*/

//global list that save EncryptFile
static list<EncryptFile *> gFileList_;

//add a pointer of EncryptFile to global list
static void addEncryptFile(EncryptFile* file) {
    if (UNLIKELY(file == NULL)) {
        return;
    }
    gFileList_.push_back(file);
}

//remove a EncryptFile from global list
static void removeEncryptFile(EncryptFile* file) {
    if (UNLIKELY(file == NULL)) {
        return;
    }
    gFileList_.remove(file);
}

//find and get a EncryptFile by the fd
static EncryptFile* getEncryptFileFromList(int fd) {
    list<EncryptFile *>::iterator iter;
    for (iter = gFileList_.begin(); iter != gFileList_.end(); iter++) {
        if ((*iter)->getFd() == fd) {
            return *iter;
        }
    }
    return NULL;
}

//throw an exception to Java framework
static void throwException(JNIEnv* env, jclass exceptionClass, jmethodID constructorWithCause,
      jmethodID constructorWithoutCause, const char* functionName, int error) {

    jthrowable cause = NULL;
    if (env->ExceptionCheck()) {
        cause = env->ExceptionOccurred();
        env->ExceptionClear();
    }
    ScopedLocalRef<jstring> detailMsg(env, env->NewStringUTF(functionName));
    if (detailMsg.get() == NULL) {
        env->ExceptionClear();
    }

    jobject exception;
    if (cause != NULL) {
        exception = env->NewObject(exceptionClass, constructorWithCause, detailMsg.get(), error, cause);
    } else {
        exception = env->NewObject(exceptionClass, constructorWithoutCause, detailMsg.get(), error);
    }

    env->Throw(reinterpret_cast<jthrowable>(exception));
}

static void throwErrnoException(JNIEnv* env, const char* functionName) {
    int error = errno;
    static jmethodID constructorWithCause = env->GetMethodID(JniConstants::errnoExceptionClass, "<init>",
          "(Ljava/lang/String;ILjava/lang/Throwable;)V");
    static jmethodID constructorWithoutCause = env->GetMethodID(JniConstants::errnoExceptionClass, "<init>",
          "(Ljava/lang/String;I)V");
    throwException(env, JniConstants::errnoExceptionClass, constructorWithCause, constructorWithoutCause, functionName, error);
}

static bool throwIfFalse(JNIEnv* env, const char* name, bool notThrow) {
    if (!notThrow) {
        throwErrnoException(env, name);
    }
    return notThrow;
}

static int throwIfMinusOne(JNIEnv* env, const char* name, int result) {
    if (result == -1) {
        throwErrnoException(env, name);
    }
    return result;
}

//generate a StructStat object
static jobject makeStructStat(JNIEnv* env, const struct stat& st) {
    jmethodID constructor = env->GetMethodID(JniConstants::structStatClass, "<init>", "(JJIJIIJJJJJJJ)V");
    return env->NewObject(JniConstants::structStatClass, constructor,
              static_cast<jlong>(st.st_dev),
              static_cast<jlong>(st.st_ino),
              static_cast<jint>(st.st_mode),
              static_cast<jlong>(st.st_nlink),
              static_cast<jint>(st.st_uid),
              static_cast<jint>(st.st_gid),
              static_cast<jlong>(st.st_rdev),
              static_cast<jlong>(st.st_size),
              static_cast<jlong>(st.st_atime),
              static_cast<jlong>(st.st_mtime),
              static_cast<jlong>(st.st_ctime),
              static_cast<jlong>(st.st_blksize),
              static_cast<jlong>(st.st_blocks));
}

static jobject doStat(JNIEnv* env, jstring javaPath, bool isLstat) {
    ScopedUtfChars path(env, javaPath);
    if (UNLIKELY(path.c_str() == NULL))
        return NULL;

    struct stat st;
    int result = isLstat ? rawLstat(path.c_str(), &st) : rawStat(path.c_str(), &st);
    if (result == -1) {
        throwErrnoException(env, isLstat ? "lstat" : "stat");
        return NULL;
    }
    return makeStructStat(env, st);
}

/*
* implements all functions in class EncryptPosix
*/

jobject EncryptPosix::open(JNIEnv* env, jobject, jstring javaPath, jint javaFlags, jint mode) {
    ScopedUtfChars path(env, javaPath);
    if (UNLIKELY(path.c_str() == NULL)) {
        return NULL;
    }
    EncryptFile* file = new EncryptFile(path.c_str());
    bool result= throwIfFalse(env, "open", file->open(javaFlags, mode));
    if (result) {
        addEncryptFile(file);
    }
    int fd = file->getFd();
    return fd != -1 ? jniCreateFileDescriptor(env, fd) : NULL;
}

void EncryptPosix::close(JNIEnv* env, jobject, jobject javaFd) {
    int fd = jniGetFDFromFileDescriptor(env, javaFd);
    jniSetFileDescriptorOfFD(env, javaFd, -1);

    EncryptFile* file = getEncryptFileFromList(fd);
    if (file == NULL) {
        rawClose(fd);
    } else {
        throwIfFalse(env, "close", file->close());
        removeEncryptFile(file);
        delete file;
        file = NULL;
    }
}

jobject EncryptPosix::stat(JNIEnv* env, jobject, jstring javaPath) {
    ScopedUtfChars path(env, javaPath);
    if (UNLIKELY(path.c_str() == NULL)) {
        return NULL;
    }
    return doStat(env, javaPath, false);
}

jobject EncryptPosix::fstat(JNIEnv* env, jobject, jobject javaFd) {
    int fd = jniGetFDFromFileDescriptor(env, javaFd);
    struct stat st;
    EncryptFile* file = getEncryptFileFromList(fd);
    if (file == NULL) {
        throwIfMinusOne(env, "fstat", rawFstat(fd, &st));
    } else {
        throwIfFalse(env, "fstat", file->fstat(&st));
    }

    return makeStructStat(env, st);
}

jint EncryptPosix::preadBytes(JNIEnv* env, jobject, jobject javaFd, jobject javaBytes,
      jint javaBytesOffset, jint javaBytesCount, jlong javaOffset) {

    ScopedBytesRW bytes(env, javaBytes);
    if (UNLIKELY(bytes.get() == NULL))
        return -1;

    int fd = jniGetFDFromFileDescriptor(env, javaFd);
    EncryptFile* file = getEncryptFileFromList(fd);
    if (file == NULL) {
        return throwIfMinusOne(env, "pread",
            rawPread(fd, reinterpret_cast<char *>(bytes.get() + javaBytesOffset), javaBytesCount, javaOffset));
    }
    return throwIfMinusOne(env, "pread",
        file->pread(reinterpret_cast<char *>(bytes.get() + javaBytesOffset), javaBytesCount, javaOffset));

}

jint EncryptPosix::pwriteBytes(JNIEnv* env, jobject, jobject javaFd, jbyteArray javaBytes,
      jint javaBytesOffset, jint javaBytesCount, jlong javaOffset) {

    ScopedBytesRO bytes(env, javaBytes);
    if (UNLIKELY(bytes.get() == NULL))
        return -1;

    int fd = jniGetFDFromFileDescriptor(env, javaFd);
    EncryptFile* file = getEncryptFileFromList(fd);
    if (file == NULL) {
        return throwIfMinusOne(env, "pwrite",
            rawPwrite(fd, reinterpret_cast<const char *>(bytes.get() + javaBytesOffset), javaBytesCount, javaOffset));
    }
    return throwIfMinusOne(env, "pwrite",
        file->pwrite(reinterpret_cast<const char *>(bytes.get() + javaBytesOffset), javaBytesCount, javaOffset));

}

jint EncryptPosix::readBytes(JNIEnv* env, jobject, jobject javaFd, jobject javaBytes,
      jint javaBytesOffset, jint javaBytesCount) {

    ScopedBytesRW bytes(env, javaBytes);
    if (UNLIKELY(bytes.get() == NULL))
        return -1;

    int fd = jniGetFDFromFileDescriptor(env, javaFd);
    EncryptFile* file = getEncryptFileFromList(fd);
    if (file == NULL) {
        return throwIfMinusOne(env, "read",
            rawRead(fd, reinterpret_cast<char *>(bytes.get() + javaBytesOffset), javaBytesCount));
    }
    return throwIfMinusOne(env, "read",
        file->read(reinterpret_cast<char *>(bytes.get() + javaBytesOffset), javaBytesCount));

}

jint EncryptPosix::writeBytes(JNIEnv* env, jobject, jobject javaFd, jbyteArray javaBytes,
      jint javaBytesOffset, jint javaBytesCount) {

    ScopedBytesRO bytes(env, javaBytes);
    if (UNLIKELY(bytes.get() == NULL))
        return -1;

    int fd = jniGetFDFromFileDescriptor(env, javaFd);
    EncryptFile* file = getEncryptFileFromList(fd);
    if (file == NULL) {
        return throwIfMinusOne(env, "write",
            rawWrite(fd, reinterpret_cast<const char *>(bytes.get() + javaBytesOffset), javaBytesCount));
    }
    return throwIfMinusOne(env, "write",
        file->write(reinterpret_cast<const char *>(bytes.get() + javaBytesOffset), javaBytesCount));

}

jlong EncryptPosix::mmap(JNIEnv* env, jobject, jlong javaAddress, jlong javaBytesCount,
      jint javaProt, jint javaFlags, jobject javaFd, jlong javaOffset) {

    int fd = jniGetFDFromFileDescriptor(env, javaFd);
    void* suggestedPtr = reinterpret_cast<void *>(static_cast<uintptr_t>(javaAddress));

    EncryptFile* file = getEncryptFileFromList(fd);
    if (file == NULL) {
        return static_cast<jlong>(reinterpret_cast<uintptr_t>(
                rawMmap(suggestedPtr, javaBytesCount, javaProt, javaFlags, fd, javaOffset)));
    }

    return static_cast<jlong>(reinterpret_cast<uintptr_t>(
            file->mmap(suggestedPtr, javaBytesCount, javaProt, javaFlags, javaOffset)));

}

void EncryptPosix::ftruncate(JNIEnv* env, jobject, jobject javaFd, jlong javaLen) {
    int fd = jniGetFDFromFileDescriptor(env, javaFd);

    EncryptFile* file = getEncryptFileFromList(fd);
    if (file == NULL) {
        throwIfMinusOne(env, "ftruncate", rawFtruncate64(fd, javaLen));
    } else {
        throwIfMinusOne(env, "ftruncate", file->ftruncate(javaLen));
    }
}

long EncryptPosix::lseek(JNIEnv* env, jobject, jobject javaFd, jlong javaOffset, jint javaWhence) {
    int fd = jniGetFDFromFileDescriptor(env, javaFd);

    EncryptFile* file = getEncryptFileFromList(fd);
    if (file == NULL) {
        return rawLseek64(fd, javaOffset, javaWhence);
    }
    return file->lseek(javaOffset, javaWhence);
}

} // namespace io
