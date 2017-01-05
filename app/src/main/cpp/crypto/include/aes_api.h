/*
* Author: wangyun
* Date: 2017-01-04
* Email: yun.wang@vmplay.com
* LastUpdateTime: 2017-01-04
* LastUpdateBy: wangyun
*/

#ifndef ENCRYPTED_AES_API_H
#define ENCRYPTED_AES_API_H

#include "block-inl.h"

using tools::Block;

typedef struct Key {
    unsigned char data[SINGLE_BLOCK_SIZE];
} Key;

namespace crypto {

class Aes {

public:
    static bool initKey(Key* key);

    static bool encrypt(const char* in, int inSize, char* out, int outSize);

    static bool decrypt(const char* in, int inSize, char* out, int outSize);
}; //class AES

} // namespace crypto

#endif
