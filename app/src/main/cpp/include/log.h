/*
* Author: wangyun
* Date: 2016-12-30
* Email: yun.wang@vmplay.com
* LastUpdateTime: 2016-12-30
* LastUpdateBy: wangyun
*/
#ifndef ENCRYPTED_LOH_H
#define ENCRYPTED_LOH_H

#include <android/log.h>

#ifndef TAG
#define TAG "encrypt_io"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define DEBUG 1

#if DEBUG
#define LOGD(...) \
  __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
#else
#define LOGD(...) \
  do {} while (0)
#endif

#if DEBUG
#define LOGE(...) \
  __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)
#else
#define LOGE(...) \
  do {} while (0)
#endif

#ifdef __cplusplus
}
#endif

#endif // ENCRYPTED_LOH_H
