/*
* Author: wangyun
* Date: 2016-12-30
* Email: yun.wang@vmplay.com
* LastUpdateTime: 2017-01-03
* LastUpdateBy: wangyun
*/
#ifndef ENCRYPTED_ENCRYPT_DATA_INL_H
#define ENCRYPTED_ENCRYPT_DATA_INL_H

#include "encrypt_data.h"
#include "encrypt_head-inl.h"

namespace io {

inline off_t EncryptData::getRealPos(){
    if (mHead_ != NULL) {
        return mPos_ + mHead_->getHeadSize();
    }
    return mPos_;
}

} // namespace io

#endif
