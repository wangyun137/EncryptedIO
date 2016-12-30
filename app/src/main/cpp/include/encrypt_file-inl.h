/*
* Author: wangyun
* Date: 2016-12-30
* Email: yun.wang@vmplay.com
* LastUpdateTime: 2016-12-30
* LastUpdateBy: wangyun
*/
#include "encrypt_file.h"

namespace io {

inline off_t getRealPos() const {
    return mData_ == NULL ? 0 : mData_->getRealPos();
}

inline int getHeadSize() const {
    return mHead_ == NULL ? 0 : mHead_->getHeadSize;
}

inline bool isEncrypted() const {
    return mHead_ == NULL ? 0 : mHead_->isEncrypted();
}

}
