/*
* Author: wangyun
* Date: 2016-12-30
* Email: yun.wang@vmplay.com
* LastUpdateTime: 2016-12-30
* LastUpdateBy: wangyun
*/

#include "encrypt_head.h"
#include "marcros.h"

namespace io {

inline void EncryptHead::clear() {
  if (LIKELY(mHead_ != NULL)) {
      free(mHead_)
  }
  mHead_ = NULL;
}

inline void setFileSizeInHead(long size) {
    if (LIKELY(mHead_ != NULL)) mHead_->fileSize = size;
}

} // namespace io
