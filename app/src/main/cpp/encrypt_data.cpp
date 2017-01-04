/*
* Author: wangyun
* Date: 2016-12-30
* Email: yun.wang@vmplay.com
* LastUpdateTime: 2017-01-03
* LastUpdateBy: wangyun
*/
#include "encrypt_data-inl.h"
#include "encrypt_utils.h"
#include "log.h"
#include "file_utils-inl.h"
#include "posix.h"
#include "marcos.h"
#include "crypto.h"

#include <errno.h>
#include <string.h>

using tools::Block;
using tools::BlockList;
using crypto::AesCrypto;

namespace io {

EncryptData::EncryptData(EncryptFile* file, EncryptHead* head)
      : mFile_(file), mHead_(head), mPos_(0) {

    mCacheList_.setLimit(100);
    mCacheList_.setCallback((CacheList<Block, BlockList>::Callback)EncryptUtils::encryptWrite, file);
}

void EncryptData::init() {
    if (LIKELY(!mHead_->isEncrypted()
          && isHasWrite(mFile_.getFlags()))) {
        mHead_->createHead();
        mHead_->flush();
    }
}

int EncryptData::read(char* buf, int size) {
    if (UNLIKELY(!mHead_->isEncrypted())) {
        return rawRead(mFile_.getFd(), buf, size);
    }

    int result = pread(buf, size, mPos_);
    if (UNLIKELY(result == -1))
        return result;

    mPos_ += result;
    return result;
}

int EncryptData::write(const char* buf, int size) {
    if (UNLIKELY(!mHead_->isEncrypted())) {
        return rawWrite(mFile_.getFd(), buf, size);
    }

    int result = pwrite(buf, size, mPos_);
    if (UNLIKELY(result == -1))
        return result;

    int realWriteSize = (result == size ? size : result);
    mPos_ += realWriteSize;
    mHead_->setFileSizeInHead(mHead_->getFileSize() + realWriteSize);
    return result;
}

int EncryptData::pread(char* buf, int size, off_t offset) {
    if (UNLIKELY(!mHead_->isEncrypted())) {
        return rawPread(mFile_.getFd(), buf, size, offset);
    }
    if (UNLIKELY(mHead_->getFileSize() - offset < size)) {
        size = mHead_->getFileSize() - offset;
    }
    if (UNLIKELY(size == 0)) {
        return 0;
    }
    if (LIKELY(isHasWrite(mFile_.getFlags()))) {
        mCacheList_.flush();
    }
    return EncryptUtils::decryptRead(mFile_, buf, size, offset);
}

int EncryptData::pwrite(const char* buf, int size, off_t offset) {
    if (UNLIKELY(!mHead_->isEncrypted())) {
        return rawPwrite(mFile_->getFd(), buf, size, offset);
    }
    if (UNLIKELY(offset == 0 && mHead_->validateFileHead(buf))) {
        mHead_->setEncrypted(false);
        mHead_->setEncrypting(true);
        return rawPwrite(mFile_->getFd(), buf, size);
    }

    BlockList blockList = generateBlockList(buf, size, offset);
    if (LIKELY(mCacheList_.isReady())) {
        mCacheList_.addList(blockList);
    } else {
        if (UNLIKELY(!EncryptUtils::encryptWrite(mFile_, blockList)))
            return -1;
    }

    return size;

}

int64_t EncryptData::lseek(int64_t offset, int32_t whence) {
    if (UNLIKELY(!mHead_->isEncrypted())) {
        int64_t pos = rawLseek64(mFile_->getFd(), offset, whence);
        return pos;
    }
    switch (whence) {
      case SEEK_SET:
        mPos_ = offset;
        break;
      case SEEK_CUR:
        mPos_ += offset;
        break;
      case SEEK_END:
        mPos_ = mHead_->getFileSize() + offset;
        break;
      default:
        break;
    }
    return mPos_;
}

void* EncryptData::mmap(void* address, int size, int prot, int flags, off_t offset) {
    if (LIKELY(isHasWrite(mFile_->getFlags()))) {
        mCacheList_.flush();
    }
    if (UNLIKELY(!mHead_->isEncrypted())) {
        return rawMmap(address, size, prot, flags, mFile_->getFd(), offset);
    }
    char* buf = (char *)calloc(1, size+1);
    if (UNLIKELY(EncryptUtils::decryptRead(mFile_, buf, size, offset) != size)) {
        return NULL;
    }
    return buf;
}


int EncryptData::ftruncate(off_T offset) {
    if (LIKELY(isHasWrite(mFile_->getFlags())))
        mCacheList_.flush();

    return rawFtruncate64(mFile_->getFd(), offset);
}

void EncryptData::close() {
    mCacheList_.flush();
}

BlockList generateBlockList(const char *buf, int size, off_t offset) {
    FileOffset fileOffset;
    EncryptUtils::computeFileOffset(&fileOffset, size, offset);

    char* writeBuf = (char *)calloc(1, fileOffset.writeSize);
    if (mCacheList_.isEmpty()) {
        mFile_.setOffset(fileOffset.startBlockOffset + mHead_->getHeadSize());
    }

    //handle the case if we need copy left data
    if (fileOffset.leftCopyLength > 0 && fileOffset.startBlockOffset < mHead_.getFileSize()) {
        Block leftBlock;
        if (!mCacheList_.isEmpty()) {
            leftBlock = mCacheList_.removeLastElement();
        } else {
            if (UNLIKELY(rawPread(mFile_.getFd(), leftBlock.getData(), SINGLE_BLOCK_SIZE,
                  fileOffset.startBlockOffset + mHead_->getHeadSize()) != SINGLE_BLOCK_SIZE)) {
                LOGE("EncryptData:generateBlockList:leftBlock: raw pread error : %s", strerror(errno));
                return BlockList();
            }
            if (UNLIKELY(!AesCrypto::decryptBlock(leftBlock))) {
                LOGE("EncryptData:generateBlockList:leftBlock: aes decrypt block error");
                return BlockList();
            }
        }
        memcpy(writeBuf, leftBlock.getData, SINGLE_BLOCK_SIZE);
        if (fileOffset.blockCount == 1) {
            memcpy(writeBuf + fileOffset.leftCopyLength, buf, size);
            return BlockUtils::toBlocks(writeBuf, fileOffset.writeSize);
        }
    }

    //handle the case if we need copy right data
    if (fileOffset.rightCopyLength > 0 && fileOffset.endPos < mHead_->getFileSize()) {
        if (!mCacheList_.isEmpty()) {
            mCacheList_.flush();
        }

        Block rightBlock;
        if (UNLIKELY(rawPread(mFile_->getFd(), rightBlock.getData(), SINGLE_BLOCK_SIZE,
              fileOffset.endBlockOffset - SINGLE_BLOCK_SIZE + mHead_->getHeadSize()) != SINGLE_BLOCK_SIZE)) {
            LOGE("EncryptData:generateBlockList:rightBlock: raw pread error: %s", strerror(errno));
            return BlockList();
        }
        if (UNLIKELY(AesCrypto::decryptBlock(rightBlock))) {
            LOGE("EncryptData:generateBlockList:rightBlock: aes decrypt block error");
            return BlockList();
        }
        memcpy(writeBuf + fileOffset.writeSize - SINGLE_BLOCK_SIZE, rightBlock.getData(), SINGLE_BLOCK_SIZE);
        if (fileOffset.blockCount == 1) {
            memcpy(writeBuf + fileOffset.leftCopyLength, buf, size);
            return BlockUtils::toBlocks(writeBuf, fileOffset.writeSize);
        }
    }

    memcpy(writeBuf + fileOffset.leftCopyLength, buf, size);
    return BlockUtils::toBlocks(writeBuf, writeSize);

}

}// namespace io
