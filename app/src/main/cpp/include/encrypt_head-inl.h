/*
* Author: wangyun
* Date: 2016-12-30
* Email: yun.wang@vmplay.com
* LastUpdateTime: 2016-12-30
* LastUpdateBy: wangyun
*/

#include "encrypt_head.h"

namespace io {

inline void EncryptHead::clear() {
  if (mHead_ != NULL) {
      free(mHead_)
  }
  mHead_ = NULL;
}

inline void setFileSizeInHead(long size) {
    if (mHead_ != NULL) mHead_->fileSize = size;
}

} // namespace io
