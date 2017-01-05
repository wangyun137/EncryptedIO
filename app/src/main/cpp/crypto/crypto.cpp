/*
* Author: wangyun
* Date: 2017-01-04
* Email: yun.wang@vmplay.com
* LastUpdateTime: 2017-01-04
* LastUpdateBy: wangyun
*/
#include "crypto.h"
#include "aes_api.h"

using tools::BlockListConstIterator;

namespace crypto {

bool AesCrypto::encryptBlock(Block &block) {
    Block out;
    bool res = Aes::encrypt(block.getConstData(), SINGLE_BLOCK_SIZE, out.getData(), SINGLE_BLOCK_SIZE);
    memcpy(block.getData(), out.getConstData(), SINGLE_BLOCK_SIZE);
    return res;
}

bool AesCrypto::decryptBlock(Block &block) {
    Block out;
    bool res = Aes::decrypt(block.getConstData(), SINGLE_BLOCK_SIZE, out.getData(), SINGLE_BLOCK_SIZE);
    memcpy(block.getData(), out.getConstData(), SINGLE_BLOCK_SIZE);
    return res;
}

bool AesCrypto::encryptBlocks(BlockList const &blockList, char *buf) {
    BlockListConstIterator iter;
    for (iter = blockList.cbegin(); iter != blockList.cend(); iter++) {
        if (UNLIKELY(!Aes::encrypt(iter->getConstData(), SINGLE_BLOCK_SIZE, buf, SINGLE_BLOCK_SIZE))) {
            return false;
        }
        buf += SINGLE_BLOCK_SIZE;
    }
    return true;
}

bool AesCrypto::decryptBlocks(BlockList const &blockList, char *buf) {
    BlockListConstIterator iter;
    for (iter = blockList.cbegin(); iter != blockList.cend(); iter++) {
        if (UNLIKELY(!Aes::decrypt(iter->getConstData(), SINGLE_BLOCK_SIZE, buf, SINGLE_BLOCK_SIZE))) {
            return false;
        }
        buf += SINGLE_BLOCK_SIZE;
    }
    return true;
}

} //namespace
