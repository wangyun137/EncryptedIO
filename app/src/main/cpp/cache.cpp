/*
* Author: wangyun
* Date: 2016-12-30
* Email: yun.wang@vmplay.com
* LastUpdateTime: 2017-01-03
* LastUpdateBy: wangyun
*/
#include "cache-inl.h"
#include "marcros.h"

namespace tools {

template<typename Item, typename List>
void Cache<Item, List>::addList(List const &listArg) {
    mCacheList_.insert(mCacheList_.end(), listArg.begin(), listArg.end());
    if (mCacheList_.size() >= mLimit_) {
        flush();
    }
}

template<typename Item, typename List>
void Cache<Item, List>::addItem(Item const &item) {
    mCacheList_.push_back(item);
    if (mCacheList_.size() >= mLimit_) {
        flush();
    }
}

template<typename Item, typename List>
void Cache<Item, List>::flush() {
    if (UNLIKELY(mCacheList_.empty()))
        return;

    if (!mCallbackFunc_(mArgs_, mCacheList_)) {
        mReady_ = false;
    } else {
        mReady_ = true;
        mCacheList_.clear();
    }
}

} // namespace tools
