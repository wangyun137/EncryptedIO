/*
* Author: wangyun
* Date: 2016-12-30
* Email: yun.wang@vmplay.com
* LastUpdateTime: 2016-12-30
* LastUpdateBy: wangyun
*/
#ifndef ENCRYPTED_ENCRYPT_HEAD_H
#define ENCRYPTED_ENCRYPT_HEAD_H

#define HEAD_RESERVED_SIZE 20
#define HEAD_MAGIC_V1 0x012fa85b
#define HEAD_VERSION_V1 0x01

namespace io {

class EncryptFile;

typedef struct FileHead {
    int magic;
    int version;
    long fileSize;
    unsigned char reserved[]
} FileHead;

class EncryptHead {

public:
    EncryptHead(EncryptFile* file) : mFile_(file), mHead_(NULL) {}

    ~EncryptHead() {
        clear();
    }

    inline void clear();

    long getFileSize() const {
        return isEncrypted() ? mHead_->fileSize : 0;
    }

    bool isEncrypted() const {
        return mEncrypted_;
    }

    void setEncrypted(bool encrypted) {
        mEncrypted_ = encrypted;
    }

    bool isEncrypting() const {
        return mEncrypting_;
    }

    void setEncrypting(bool encrypting) {
        mEncrypting_ = encrypting;
    }

    int getHeadSize() const {
        return sizeof(FileHead);
    }

    void* getHead() const {
        return mHead_;
    }

    inline void setFileSizeInHead(long size);

    void readHead();

    void createHead();

    bool flush() const;

    void printFileHead() const;

    bool validateFileHead(const void *head);

private:
    EncryptFile* mFile_;

    FileHead* mHead_;

    bool mEncrypted_;

    bool mEncrypting_;

    FileHead* allocFileHead() const;

}; // class EncryptHead

} // namespace io

#endif // ENCRYPTED_ENCRYPT_HEAD_H
