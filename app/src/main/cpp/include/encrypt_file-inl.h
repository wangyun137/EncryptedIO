/*
* Author: wangyun
* Date: 2016-12-30
* Email: yun.wang@vmplay.com
* LastUpdateTime: 2016-12-30
* LastUpdateBy: wangyun
*/
#include "encrypt_file.h"
#include "marcos.h"

namespace io {

inline off_t getRealPos() const {
    return LIKELY(mData_ != NULL) ? mData_->getRealPos() : 0;
}

inline int getHeadSize() const {
    return LIKELY(mHead_ != NULL) ? mHead_->getHeadSize : 0;
}

inline bool isEncrypted() const {
    return LIKELY(mHead_ != NULL) ? mHead_->isEncrypted() : 0;
}

}
