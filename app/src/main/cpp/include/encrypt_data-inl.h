/*
* Author: wangyun
* Date: 2016-12-30
* Email: yun.wang@vmplay.com
* LastUpdateTime: 2017-01-03
* LastUpdateBy: wangyun
*/
#include "encrypt_data.h"

namespace io {

inline off_t EncryptData::getRealPos(){
    return mPos_ + mHead_ != NULL ? mHead_->headSize : 0;
}

inline int EncryptData::getCeilBlockNum(off_t offset) {
    return (offset + SINGLE_BLOCK_SIZE - 1) / SINGLE_BLOCK_SIZE;
}

inline int EncryptData::getFloorBlockNum(off_t offset) {
    return offset / SINGLE_BLOCK_SIZE;
}

} // namespace io
