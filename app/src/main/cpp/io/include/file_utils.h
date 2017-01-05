/*
* Author: wangyun
* Date: 2016-12-30
* Email: yun.wang@vmplay.com
* LastUpdateTime: 2016-12-30
* LastUpdateBy: wangyun
*/
#ifndef ENCRYPT_FILE_UTILS_H
#define ENCRYPT_FILE_UTILS_H

#include <fcntl.h>
#include <sys/stat.h>

inline bool isWriteOnly(int flags);

inline bool isReadOnly(int flags);

inline bool isReadWrite(int flags);

inline int setReadWrite(int flags);

inline int setReadOnly(int flags);

inline int setWriteOnly(int flags);

inline bool isDir(struct stat *st);

inline bool isHasWrite(int flags);

#endif // ENCRYPT_FILE_UTILS_H
