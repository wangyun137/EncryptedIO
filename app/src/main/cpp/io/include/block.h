/*
* Author: wangyun
* Date: 2016-12-30
* Email: yun.wang@vmplay.com
* LastUpdateTime: 2017-01-03
* LastUpdateBy: wangyun
*/

#ifndef ENCRYPTED_BLOCK_H
#define ENCRYPTED_BLOCK_H

#include "macros.h"

#include <list>
#include <stdlib.h>
#include <string.h>

#define SINGLE_BLOCK_SIZE 16

using std::list;

namespace tools {

class Block {

public:
    Block() {
        mData_ = (char *)calloc(1, SINGLE_BLOCK_SIZE + 1);
    }

    Block(const char* buf, int copySize) {
        mData_ = (char *)calloc(1, SINGLE_BLOCK_SIZE + 1);
        memcpy(mData_, buf, copySize);
    }

    Block(Block const &block) {
        mData_ = (char *)calloc(1, SINGLE_BLOCK_SIZE + 1);
        memcpy(mData_, block.getConstData(), SINGLE_BLOCK_SIZE);
    }

    ~Block() {
        clear();
    }

    Block& operator=(Block const &other) {
        memset(mData_, 0, SINGLE_BLOCK_SIZE);
        memcpy(mData_, other.getConstData(), SINGLE_BLOCK_SIZE);
        return *this;
    }

    inline const char* getConstData() const ;

    inline char* getData();

    inline void clear();

private:
    char* mData_;
}; //

typedef list<Block> BlockList;
typedef list<Block>::const_iterator BlockListConstIterator;

class BlockUtils {

public:
    static BlockList toBlocks(const char* buf, int size) {
        int count = size / SINGLE_BLOCK_SIZE;
        int rest = size % SINGLE_BLOCK_SIZE;
        BlockList blocks;

        for (int i = 0; i < count; i++) {
            blocks.push_back(Block(buf, SINGLE_BLOCK_SIZE));
            buf += SINGLE_BLOCK_SIZE;
        }
        if (rest > 0) {
            blocks.push_back(Block(buf, SINGLE_BLOCK_SIZE));
        }
        return blocks;
    }

    static BlockList toBlocks(const char* buf) {
        return toBlocks(buf, (int)strlen(buf));
    }
};

} // namespace tools

#endif // ENCRYPTED_BLOCK_H
