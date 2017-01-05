/*
* Author: wangyun
* Date: 2016-12-30
* Email: yun.wang@vmplay.com
* LastUpdateTime: 2016-12-30
* LastUpdateBy: wangyun
*/
#ifndef ENCRYPTED_ENCRYPT_DATA_H
#define ENCRYPTED_ENCRYPT_DATA_H

#include "cache-inl.h"
#include "block-inl.h"

using namespace tools;

namespace io {

class EncryptFile;
class EncryptHead;

class EncryptData {
public:

    EncryptData(EncryptFile* file, EncryptHead* head);

    ~EncryptData(){}

    void init();

    int read(char* buf, int size);

    int write(const char* buf, int size);

    int pread(char* buf, int size, off_t offset);

    int pwrite(const char* buf, int size, off_t offset);

    int64_t lseek(int64_t offset, int32_t whence);

    void* mmap(void* address, int size, int prot, int flags, off_t offset);

    int ftruncate(off_t offset);

    void close();

    inline off_t getRealPos();

private:
    EncryptHead *mHead_;

    EncryptFile *mFile_;

    off_t mPos_;

    CacheList<Block, BlockList> mCacheList_;

    BlockList generateBlockList(const char *buf, int size, off_t offset);

}; // class EncryptData

} // namespace io

#endif // ENCRYPTED_ENCRYPT_DATA_H
