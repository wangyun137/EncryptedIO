/*
* Author: wangyun
* Date: 2016-12-30
* Email: yun.wang@vmplay.com
* LastUpdateTime: 2016-12-30
* LastUpdateBy: wangyun
*/
#include "encrypt_utils.h"
#include "log.h"
#include "posix.h"
#include "marcos.h"
#include "crypto.h"

#include <errno.h>
#include <string.h>

using tools::BlockList;
using crypto::AesCrypto;

namespace io {

static void EncryptUtils::computeFileOffset(FileOffset* fileOffset, int size, off_t offset) {
    if (UNLIKELY(fileOffset == NULL))
        return;

    fileOffset->startPos = offset;
    fileOffset->endPos = offset + size;
    fileOffset->startBlockNum = getFloorBlockNum(startPos);
    fileOffset->endBlockNum = getCeilBlockNum(endPos);
    fileOffset->blockCount = endBlockNum - startBlockNum;
    fileOffset->writeSize = blockCount * SINGLE_BLOCK_SIZE;
    fileOffset->readSize = (endBlockNum - startBlockNum) * SINGLE_BLOCK_SIZE;
    fileOffset->startBlockOffset = startBlockNum * SINGLE_BLOCK_SIZE;
    fileOffset->endBlockOffset = endBlockNum * SINGLE_BLOCK_SIZE;
    fileOffset->leftCopyLength = startPos - startBlockOffset;
    fileOffset->rightCopyLength = endBlockOffset - endPos;

    printFileOffset(fileOffset);
}

static void printFileOffset(FileOffset* fileOffset) {
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

static int EncryptUtils::decryptRead(EncryptFile* file, void *buf, int size, off_t offset) {
    if (UNLIKELY(file == NULL)) {
        return -1;
    }
    FileOffset fileOffset;
    computeFileOffset(&fileOffset, size, offset);
    char* readBuf = (char *)calloc(1, fileOffset.readSize);
    if (UNLIKELY(rawPread(file.getFd(), readBuf, readSize,
          file.getHeadSize() + fileOffset.startBlockOffset) != readSize)) {
        LOGE("EncryptUtils:decryptRead: raw read error : %s", strerror(errno));
        return -1;
    }
    BlockList blockList = BlockUtils::toBlocks(readBuf, readSize);
    memset(readBuf, 0, readSize);
    AesCrypto::decryptBlocks(blockList, readBuf);
    memcpy(buf, readBuf + fileOffset.leftCopyLength, size);
    return size;
}

static bool EncryptUtils::encryptWrite(void *args, BlockList &blockList) {
    EncryptFile* file = reinterpret_cast<EncryptFile *>(args);
    int size = blockList.size() * SINGLE_BLOCK_SIZE;
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
