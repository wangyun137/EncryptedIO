/*
* Author: wangyun
* Date: 2016-12-30
* Email: yun.wang@vmplay.com
* LastUpdateTime: 2016-12-30
* LastUpdateBy: wangyun
*/

#ifndef ENCRYPTED_ENCRYPT_HEAD_INL_H
#define ENCRYPTED_ENCRYPT_HEAD_INL_H

#include <malloc.h>
#include "encrypt_head.h"
#include "macros.h"

namespace io {

inline void EncryptHead::clear() {
  if (LIKELY(mHead_ != NULL)) {
      free(mHead_);
  }
  mHead_ = NULL;
}

inline void EncryptHead::setFileSizeInHead(long size) {
    if (LIKELY(mHead_ != NULL)) mHead_->fileSize = size;
}

} // namespace io

#endif
