/*
* Author: wangyun
* Date: 2016-12-30
* Email: yun.wang@vmplay.com
* LastUpdateTime: 2016-12-30
* LastUpdateBy: wangyun
*/
#include "file_utils.h"

inline bool isWriteOnly(int flags) {
    return (flags & O_ACCMODE) == O_WRONLY;
}

inline bool isReadOnly(int flags){
    return (flags & O_ACCMODE) == O_RDONLY;
}

inline bool isReadWrite(int flags) {
    return (flags & O_ACCMODE) == O_RDWR;
}

inline bool isNeedCreate(int flags) {
    return (flags & O_CREATE) == O_CREATE;
}

inline int setReadWrite(int flags) {
    if (isReadOnly(flags)) {
        return (flags & ~O_RDONLY) | O_RDWR;
    }
    if (isWriteOnly(flags) {
        return (flags & ~O_WRONLY) | O_RDWR;
    }
    return flags;
}

inline int setReadOnly(int flags) {
    if (isWriteOnly(flags)) {
        return (flags & ~O_WRONLY) | O_RDONLY;
    }
    if (isReadWrite(flags)) {
        return (flags & ~O_RDWR) | O_RDONLY;
    }
    return flags;
}

inline int setWriteOnly(int flags) {
    if (isReadOnly(flags)) {
        return (flags & ~O_RDONLY) | O_WRONLY;
    }
    if (isReadWrite(flags)) {
        return (flags & ~O_RDWR) | O_RDONLY;
    }
    return flags;
}

inline bool isDir(struct stat *st) {
    return S_ISDIR(st->st_mode) == 1;
}

inline bool isHasWrite(int flags) {
    return isWriteOnly(flags) || isReadWrite(flags);
}
