/*
* Author: wangyun
* Date: 2016-12-30
* Email: yun.wang@vmplay.com
* LastUpdateTime: 2017-01-03
* LastUpdateBy: wangyun
*/
#ifndef ENCRYPTED_BLOCK_INL_H
#define ENCRYPTED_BLOCK_INL_H

#include "block.h"

namespace tools {

inline const char* Block::getConstData() const {
    return (const char *)mData_;
}

inline char* Block::getData() {
    return mData_;
}

inline void Block::clear() {
    if (LIKELY(mData_ != NULL))
        free(mData_);
    mData_ = NULL;
}

}

#endif
