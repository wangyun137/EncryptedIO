/*
* Author: wangyun
* Date: 2016-12-30
* Email: yun.wang@vmplay.com
* LastUpdateTime: 2016-12-30
* LastUpdateBy: wangyun
*/
#ifndef ENCRYPTED_ENCRYPT_UTILS_H
#define ENCRYPTED_ENCRYPT_UTILS_H

#include "block-inl.h"

using tools::BlockList;

namespace io {

typedef struct FileOffset {
    off_t startPos;
    off_t endPos;
    off_t startBlockOffset;
    off_t endBlockOffset;
    int startBlockNum;
    int endBlockNum;
    int blockCount;
    int writeSize;
    int readSize;
    int leftCopyLength;
    int rightCopyLength;
} FileOffset;

class EncryptFile;

class EncryptUtils {

public:
    static void computeFileOffset(FileOffset* fileOffset, int size, off_t offset);

    static int decryptRead(EncryptFile* file, void *buf, int size, off_t offset);

    static bool encryptWrite(void *args, BlockList &blockList);
}// class EncryptUtils

}

#endif // ENCRYPTED_ENCRYPT_UTILS_H
