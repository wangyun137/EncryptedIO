/*
* Author: wangyun
* Date: 2016-12-30
* Email: yun.wang@vmplay.com
* LastUpdateTime: 2016-12-30
* LastUpdateBy: wangyun
*/
#ifndef ENCRYPTED_POSIX_H
#define ENCRYPTED_POSIX_H

#include <sys/stat.h>

int rawOpen(const char *path_name, int flags, int mode);

void rawClose(int fd);

int rawPread(int fd, void *buf, int size, off_t offset);

int rawRead(int fd, void *buf, int size);

int rawPwrite(int fd, const void *buf, int size, off_t offset);

int rawWrite(int fd, const void *buf, int size);

int rawFstat(int fd, struct stat *st);

int rawStat(const char *path_name, struct stat *st);

int rawLstat(const char *path_name, struct stat *st);

int rawIoctl(int fd, int cmd, void *arg);

int rawFtruncate(int fd, off_t offset);

int32_t rawLseek(int fd, int32_t offset, int32_t whence);

int rawRename(const char *old_name, const char *new_name);

int rawRemove(const char *path);

void* rawMmap(void *address, int size, int prot, int flags, off_t offset);

int rawMunmap(void *address, int size);

long rawSize(int fd);

#endif //ENCRYPTED_POSIX_H
