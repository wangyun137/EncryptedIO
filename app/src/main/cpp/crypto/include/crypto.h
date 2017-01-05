/*
* Author: wangyun
* Date: 2017-01-04
* Email: yun.wang@vmplay.com
* LastUpdateTime: 2017-01-04
* LastUpdateBy: wangyun
*/
#ifndef ENCRYPTED_CRYPTO_H
#define ENCRYPTED_CRYPTO_H

#include "block-inl.h"

using tools::Block;
using tools::BlockList;

namespace crypto {

class AesCrypto {

public:
    static bool encryptBlock(Block &block);

    static bool decryptBlock(Block &block);

    static bool encryptBlocks(BlockList const &blockList, char *buf);

    static bool decryptBlocks(BlockList const &blockList, char *buf);

}; // class AesCrypto

} // namespace crypto

#endif // end ENCRYPTED_CRYPTO_H
