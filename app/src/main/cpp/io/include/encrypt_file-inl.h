/*
* Author: wangyun
* Date: 2016-12-30
* Email: yun.wang@vmplay.com
* LastUpdateTime: 2016-12-30
* LastUpdateBy: wangyun
*/
#ifndef ENCRYPTED_ENCRYPT_FILE_INL_H
#define ENCRYPTED_ENCRYPT_FILE_INL_H

#include "encrypt_file.h"
#include "encrypt_data-inl.h"

namespace io {

inline int EncryptFile::getFd() const {
	return mFd_;
}

inline int EncryptFile::getFlags() const {
	return mFlags_;
}

inline off_t EncryptFile::getRealPos() const {
		if (mData_ == NULL) {
				return 0;
		}
    return mData_->getRealPos();
}

inline int EncryptFile::getHeadSize() const {
		if (mHead_ == NULL) {
				return 0;
		}
    return mHead_->getHeadSize();
}

inline bool EncryptFile::isEncrypted() const {
		if (mHead_ == NULL) {
				return false;
		}
    return mHead_->isEncrypted();
}

}

#endif
