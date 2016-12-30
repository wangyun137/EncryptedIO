/*
* Author: wangyun
* Date: 2016-12-30
* Email: yun.wang@vmplay.com
* LastUpdateTime: 2016-12-30
* LastUpdateBy: wangyun
*/
#ifndef ENCRYPTED_ENCRYPT_FILE_H
#define ENCRYPTED_ENCRYPT_FILE_H

#include <string>
#include <sys/stat.h>

namespace io {

class EncryptHead;
class EncryptData;

class EncryptFile {

public:
    EncryptFile(const char *path) :
        mHead_(NULL), mData_(NULL), mFd_(-1),
        mFlags_(0), mOffset_(0) {}

    ~EncryptFile() {
        close();
    }

    int getFd() const {
        return mFd_;
    }

    int getFlags() const {
        return mFlags_;
    }

    inline off_t getRealPos() const;

    inline int getHeadSize() const;

    inline bool isEncrypted() const;

    long getOffset() const {
        return mOffset_;
    }

    void setOffset(long offset) {
        mOffset_ = offset;
    }

    bool open(int flags, int mode);

    bool close();

    int ftruncate(long offset);

    int read(void *buf, int size);

    int write(const void *buf, int size);

    int pread(void *buf, int size, long offset);

    int pwrite(const void *buf, int size, long offset);

    long seek(long offset);

    bool fstat(struct stat *st);

    void* mmap(void *address, int size, int prot, int flags, long offset);

    bool munmap(void *address, int size);

    int ioctl(int cmd, void *args);

    int64_t lseek(int64_t offset, int32_t whence);


private:
    string mPath_;

    int mFd_;

    int mFlags_;

    EncryptHead* mHead_;

    EncryptData* mData_;

    long mOffset_;
}; // EncryptFile

} // namespace io

#endif // ENCRYPTED_ENCRYPT_FILE_H
