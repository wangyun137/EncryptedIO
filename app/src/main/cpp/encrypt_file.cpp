/*
* Author: wangyun
* Date: 2016-12-30
* Email: yun.wang@vmplay.com
* LastUpdateTime: 2016-12-30
* LastUpdateBy: wangyun
*/
#include "encrypt_file-inl.h"
#include "posix.h"
#include "file_utils-inl.h"
#include "log.h"

namespace io {

bool EncryptFile::open(int flags, int mode) {
    if (isWriteOnly(flags)) {
        flags = setReadWrite(flags);
    }

    mFlags_ = flags;
    mFd_ = rawOpen(mPath_.c_str(), flags, mode);
    if (UNLIKELY(mFd_ == -1)) {
        LOGE("EncryptFile open: open file error: %s", strerror(errno));
        return false;
    }

    mHead_ = new EncryptHead(this);
    mHead_->read();

    mData_ = new EncryptData(this, mHead_);
    mData_->init();

    return true;
}

bool EncryptFile::close() {
    if (LIKELY(mHead_ != NULL)) {
        mHead_->close();
        delete mHead_;
        mHead_ = NULL;
    }
    if (LIKELY(mData_ != NULL)) {
        mData_->close();
        delete mData_;
        mData_ = NULL;
    }
    rawClose(mFd_);
    return true;
}

int EncryptFile::ftruncate(long offset) {
    return LIKELY(mData_ != NULL) ? mData_->ftruncate(offset) : -1;
}

int EncryptFile::read(void* buf, int size) {
    return LIKELY(mData_ != NULL) ? mData_->read((char *)buf, size) : -1;
}

int EncryptFile::write(const void* buf, int size) {
    return LIKELY(mData_ != NULL) ? mData_->write((const char *)buf, size) : -1;
}

int EncryptFile::pread(void *buf, int size, long offset) {
    return LIKELY(mData_ != NULL) ? mData_->pread((char *)buf, size, offset) : -1;
}

int EncryptFile::pwrite(const void *buf, int size, long offset) {
    return LIKELY(mData_ != NULL) ? mData_->pwrite((const char *)buf, size, offset) : -1;
}

bool EncryptFile::fstat(struct stat *st) {
    bool result = (rawFstat(mFd_, st) == 0);
    if (LIKELY(result && mHead_->isEncrypted)) {
        st->st_size = mHead_->getFileSize();
    }
    return result;
}

void* EncryptFile::mmap(void *address, int size, int prot, int flags, long offset) {
    return LIKELY(mData_ != NULL) ? mData_->mmap(address, size, prot, flags, offset) : NULL;
}

int64_t EncryptFile::lseek(int64_t offset, int32_t whence) {
    return LIKELY(mData_ != NULL) ? mData_->lseek(offset, whence) : -1;
}

} //namespace io
