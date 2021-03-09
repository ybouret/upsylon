
//! \file
#ifndef Y_SEQUENCE_CRATES_INCLUDED
#define Y_SEQUENCE_CRATES_INCLUDED 1

#include "y/container/sequence.hpp"
#include "y/type/collapse.hpp"
#include "y/memory/allocator/global.hpp"
#include "y/type/block/zset.hpp"
#include <iostream>

namespace upsylon
{

    namespace core
    {

    }

    template <typename T, typename ALLOCATOR = memory::global >
    class crates : public sequence<T>
    {
    public:

    private:
        Y_DISABLE_ASSIGN(crates);
    };
    
}

#endif
