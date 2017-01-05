/*
* Author: wangyun
* Date: 2016-12-30
* Email: yun.wang@vmplay.com
* LastUpdateTime: 2017-01-03
* LastUpdateBy: wangyun
*/
#ifndef ENCRYPTED_CACHE_INL_H
#define ENCRYPTED_CACHE_INL_H

#include "cache.h"

namespace tools {

template<typename Item, typename List>
inline bool CacheList<Item, List>::isReady() {
    return mReady_;
}

template<typename Item, typename List>
inline bool CacheList<Item, List>::isEmpty() {
    return mCacheList_.empty();
}

template<typename Item, typename List>
inline void CacheList<Item, List>::clear() {
    return mCacheList_.clear();
}

template<typename Item, typename List>
inline Item CacheList<Item, List>::getLastElement() {
    return mCacheList_.back();
}

template<typename Item, typename List>
inline Item CacheList<Item, List>::getFirstElement() {
    return mCacheList_.front();
}

template<typename Item, typename List>
inline Item CacheList<Item, List>::removeLastElement() {
    Item item = mCacheList_.back();
    mCacheList_.pop_back();
    return item;
}

template<typename Item, typename List>
inline list<Item> CacheList<Item, List>::getCacheData() {
    return mCacheList_;
}

template<typename Item, typename List>
inline void CacheList<Item, List>::setLimit(unsigned limit) {
    mLimit_ = limit;
}

template<typename Item, typename List>
inline void CacheList<Item, List>::setCallback(Callback func, void *args) {
    mCallbackFunc_ = func;
    mArgs_ = args;
}

} // namespace cache

#endif
