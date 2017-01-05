/*
* Author: wangyun
* Date: 2016-12-30
* Email: yun.wang@vmplay.com
* LastUpdateTime: 2016-12-30
* LastUpdateBy: wangyun
*/

#include "encrypt_head-inl.h"
#include "encrypt_file-inl.h"
#include "posix.h"
#include "file_utils-inl.h"
#include "log.h"

#include <stdlib.h>
#include <errno.h>

namespace io {

void EncryptHead::readHead() {
    mEncrypted_ = false;

    int headSize = getHeadSize();
    struct stat st;
    if (UNLIKELY(rawFstat(mFile_->getFd(), &st) == -1)) {
        LOGE("EncryptHead readHead: fstat error: %s", strerror(errno));
        return;
    }

    if (UNLIKELY(isDir(&st))) {
        LOGE("EncryptHead readHead: this file is directory");
        return;
    }

    if (st.st_size - headSize < 0) {
        LOGE("EncryptHead readHead: this file has not been encrypted");
        return;
    }

    rawLseek(mFile_->getFd(), 0, SEEK_SET);
    char headBuff[headSize + 1];
    memset(headBuff, 0, headSize + 1);
    if (UNLIKELY(rawPread(mFile_->getFd(), headBuff, headSize, 0) != headSize)) {
        LOGE("EncryptHead read: raw read head error");
        return;
    }
    rawLseek(mFile_->getFd(), 0, SEEK_SET);

    mEncrypted_ = validateFileHead(headBuff);
}

void EncryptHead::createHead() {
    //we should clear the mHead_ first if mHead_ point to a real obj
    clear();
    //alloc a new FileHead
    mHead_ = allocFileHead();
    mHead_->fileSize = rawSize(mFile_->getFd());
    mEncrypted_ = true;
}

bool EncryptHead::flush() const {
    printFileHead();

    if (UNLIKELY(mEncrypting_)) {
        return true;
    }
    //if the mHead_ is null, just do nothing
    if (UNLIKELY(mHead_ == NULL)) {
        LOGE("EncryptHead flush error: head is null");
        return true;
    }

    return rawPwrite(mFile_->getFd(), mHead_, getHeadSize(), 0) == getHeadSize();
}

void EncryptHead::printFileHead() const {
    if (UNLIKELY(mHead_ == NULL)) {
        LOGE("EncryptHead printFileHead: there is no FileHead");
        return;
    }
    LOGD("*******************************");
    LOGD("EncryptHead:");
    LOGD("magic : %x", mHead_->magic);
    LOGD("version : %x", mHead_->version);
    LOGD("fileSize : %ld", mHead_->fileSize);
    LOGD("*******************************");
}

FileHead* EncryptHead::allocFileHead() const{
    FileHead *head = (FileHead *)calloc(1, sizeof(FileHead));
    head->magic = HEAD_MAGIC_V1;
    head->version = HEAD_VERSION_V1;
    return head;
}

bool EncryptHead::validateFileHead(const void *headData) {
    if (UNLIKELY(headData == NULL)) {
        return false;
    }

    int headSize = sizeof(FileHead);
    FileHead* head = (FileHead *)calloc(1, headSize);
    memset(head, 0, headSize);
    memcpy(head, headData, headSize);

    if (head->magic == HEAD_MAGIC_V1 && head->version == HEAD_VERSION_V1) {
        mHead_ = head;
        return true;
    }

    free(head);
    mHead_ = NULL;
    return false;
}


} // namespace io
