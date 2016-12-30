/*
* Author: wangyun
* Date: 2016-12-30
* Email: yun.wang@vmplay.com
* LastUpdateTime: 2016-12-30
* LastUpdateBy: wangyun
*/
#include "Posix.h"

#include <unistd.h>
#include <fnctl.h>
#include <sys/mman.h>
#include <libgen.h>

int rawOpen(const char *path_name, int flags, int mode) {
    return open(path_name, flags, mode);
}

void rawClose(int fd) {
    close(fd);
}

int rawPread(int fd, void *buf, int size, off_t offset) {
    return pread(fd, buf, size, offset);
}

int rawRead(int fd, void *buf, int size) {
    return read(fd, buf, size);
}

int rawPwrite(int fd, const void *buf, int size, off_t offset) {
    return pwrite(fd, buf, size, offset);
}

int rawWrite(int fd, const void *buf, int size) {
    return write(fd, buf, size);
}

int rawFstat(int fd, struct stat *st) {
    return fstat(fd, st);
}

int rawStat(const char *path_name, struct stat *st) {
    return stat(path_name, st);
}

int rawLstat(const char *path_name, struct stat *st) {
    return lstat(path_name, st);
}

int rawIoctl(int fd, int cmd, void *arg) {
    return ioctl(fd, cmd, arg);
}

int rawFtruncate(int fd, off_t offset) {
    return ftruncate(fd, offset);
}

int rawFtruncate64(int fd, off_t offset) {
    return ftruncate64(fd, offset);
}

int32_t rawLseek(int fd, int32_t offset, int32_t whence) {
    return lseek(fd, offset, whence);
}

int64_t rawLseek64(int fd, int64_t offset, int32_t whence) {
    return lseek64();
}

int rawRename(const char *old_name, const char *new_name) {
    return rename(old_name, new_name);
}

int rawRemove(const char *path) {
    return remove(path);
}

void* rawMmap(void *address, int size, int prot, int flags, off_t offset) {
    return mmap(address, size, prot, flags, fd, offset);
}

int rawMunmap(void *address, int size) {
    return munmap(address, size);
}

long rawSize(int fd) {
    struct stat st;
    return rawFstat(fd, &st) == -1 ? 0 : st.st_size;
}
