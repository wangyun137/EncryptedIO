/*
* Author: wangyun
* Date: 2016-12-30
* Email: yun.wang@vmplay.com
* LastUpdateTime: 2016-12-30
* LastUpdateBy: wangyun
*/
#include "encrypt_utils.h"
#include "encrypt_file-inl.h"
#include "log.h"
#include "posix.h"
#include "crypto.h"

using tools::BlockList;
using tools::BlockUtils;
using crypto::AesCrypto;

namespace io {

int EncryptUtils::getCeilBlockNum(off_t offset){
    return (offset + SINGLE_BLOCK_SIZE - 1) / SINGLE_BLOCK_SIZE;
}

int EncryptUtils::getFloorBlockNum(off_t offset){
    return offset / SINGLE_BLOCK_SIZE;
}

void EncryptUtils::computeFileOffset(FileOffset* fileOffset, int size, off_t offset) {
    if (UNLIKELY(fileOffset == NULL))
        return;

    fileOffset->startPos = offset;
    fileOffset->endPos = offset + size;
    fileOffset->startBlockNum = getFloorBlockNum(fileOffset->startPos);
    fileOffset->endBlockNum = getCeilBlockNum(fileOffset->endPos);
    fileOffset->blockCount = fileOffset->endBlockNum - fileOffset->startBlockNum;
    fileOffset->writeSize = fileOffset->blockCount * SINGLE_BLOCK_SIZE;
    fileOffset->readSize = (fileOffset->endBlockNum - fileOffset->startBlockNum) * SINGLE_BLOCK_SIZE;
    fileOffset->startBlockOffset = fileOffset->startBlockNum * SINGLE_BLOCK_SIZE;
    fileOffset->endBlockOffset = fileOffset->endBlockNum * SINGLE_BLOCK_SIZE;
    fileOffset->leftCopyLength = fileOffset->startPos - fileOffset->startBlockOffset;
    fileOffset->rightCopyLength = fileOffset->endBlockOffset - fileOffset->endPos;

    printFileOffset(fileOffset);
}

void EncryptUtils::printFileOffset(FileOffset* fileOffset) {
    if (UNLIKELY(fileOffset == NULL)) {
        return;
    }
    LOGD("*************FileOffset*****************");
    LOGD("**        startPos is %ld           **", fileOffset->startPos);
    LOGD("**        endPos is %ld             **", fileOffset->endPos);
    LOGD("**        startBlockNum is %d       **", fileOffset->startBlockNum);
    LOGD("**        endBlockNum is %d         **", fileOffset->endBlockNum);
    LOGD("**        blockCount is %d          **", fileOffset->blockCount);
    LOGD("**        writeSize is %d           **", fileOffset->writeSize);
    LOGD("**        startBlockOffset is %ld   **", fileOffset->startBlockOffset);
    LOGD("**        endBlockOffset is %ld     **", fileOffset->endBlockOffset);
    LOGD("**        leftCopyLength is %d      **", fileOffset->leftCopyLength);
    LOGD("**        rightCopyLength is %d     **", fileOffset->rightCopyLength);
    LOGD("****************************************");
}

int EncryptUtils::decryptRead(EncryptFile* file, void *buf, int size, off_t offset) {
    if (UNLIKELY(file == NULL)) {
        return -1;
    }
    FileOffset fileOffset;
    computeFileOffset(&fileOffset, size, offset);
    char* readBuf = (char *)calloc(1, fileOffset.readSize);
    if (rawPread(file->getFd(), readBuf, fileOffset.readSize,
          file->getHeadSize() + fileOffset.startBlockOffset) != fileOffset.readSize) {
        LOGE("EncryptUtils:decryptRead: raw read error : %s", strerror(errno));
        return -1;
    }
    BlockList blockList = BlockUtils::toBlocks(readBuf, fileOffset.readSize);
    memset(readBuf, 0, fileOffset.readSize);
    AesCrypto::decryptBlocks(blockList, readBuf);
    memcpy(buf, readBuf + fileOffset.leftCopyLength, size);
    return size;
}

bool EncryptUtils::encryptWrite(void *args, BlockList &blockList) {
    EncryptFile* file = reinterpret_cast<EncryptFile *>(args);
    int size = blockList.size() * SINGLE_BLOCK_SIZE;
    char* buf = (char *)calloc(1, size);
    if (UNLIKELY(!AesCrypto::encryptBlocks(blockList, buf))) {
        LOGE("EncryptUtils:encryptWrite: encrypt blocks error");
        return false;
    }
    if (UNLIKELY(rawPwrite(file->getFd(), buf, size, file->getOffset()) != size)) {
        LOGE("EncryptUtils:encryptWrite: raw pwrite error: %s", strerror(errno));
        return false;
    }

    file->setOffset(-1);
    return true;
}

}
