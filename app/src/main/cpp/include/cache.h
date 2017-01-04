/*
* Author: wangyun
* Date: 2016-12-30
* Email: yun.wang@vmplay.com
* LastUpdateTime: 2017-01-03
* LastUpdateBy: wangyun
*/

#ifndef ENCRYPTED_CACHE_H
#define ENCRYPTED_CACHE_H

#include <map>
#include <list>
#include <iterator>

using std::map;
using std::pair;
using std::list;
using std::iterator;

namespace tools {

template<typename Item, typename List>
class CacheList {
public:
    typedef bool (*Callback)(void *args, List &listArg);

    CacheList() :  mLimit_(0), mReady_(true) {}

    ~CacheList() {}

    inline bool isReady();

    inline bool isEmpty();

    inline void clear();

    inline Item getLastElement();

    inline Item getFirstElement();

    inline Item removeLastElement();

    inline list<Item> getCacheData();

    inline void setLimit(unsigned limit);

    inline void setCallback(Callback func, void *args);

    void addList(List const &listArg);

    void addItem(Item const &item);

    void flush();

private:
    Callback mCallbackFunc_;
    void* mArgs_;
    list<Item> mCacheList_;
    unsigned mLimit_;
    bool mReady_;
}; // class Cache

} // namespace cache

#endif // end of ENCRYPTED_CACHE_H
