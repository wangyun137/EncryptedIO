/*
* Author: wangyun
* Date: 2017-01-04
* Email: yun.wang@vmplay.com
* LastUpdateTime: 2017-01-04
* LastUpdateBy: wangyun
*/
#include "aes_api.h"
#include "io.h"
#include "openssl/aes.h"

namespace crypto {

bool Aes::initKey(Key* key) {
    if (key == NULL) {
        return false;
    }
    const char* ioKey = io::IO::getEncryptKey();
    int i;
    if (ioKey != NULL) {
        for (i = 0; i < SINGLE_BLOCK_SIZE; i++) {
            key->data[i] = ioKey[i];
        }
    } else {
        for (i = 0; i < SINGLE_BLOCK_SIZE; i++) {
            key->data[i] = 32 + i;
        }
    }
    return true;
}

bool Aes::encrypt(const char* in, int inSize, char* out, int outSize) {
    Key key;
    AES_KEY aesKey;
    if (UNLIKELY(!initKey(&key))) {
        return false;
    }
    AES_set_encrypt_key(key.data, 128, &aesKey);
    AES_encrypt((unsigned char*)in, (unsigned char*)out, &aesKey);
    return true;
}

bool Aes::decrypt(const char* in, int inSize, char* out, int outSize) {
    Key key;
    AES_KEY aesKey;
    if (UNLIKELY(!initKey(&key))) {
        return false;
    }
    AES_set_decrypt_key(key.data, 128, &aesKey);
    AES_decrypt((unsigned char*)in, (unsigned char*)out, &aesKey);
    return true;
}

}
