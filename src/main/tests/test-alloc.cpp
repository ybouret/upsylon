#include "y/memory/global.hpp"
#include "y/utest/run.hpp"
#include "y/random/bits.hpp"

using namespace upsylon;

namespace
{
    struct block
    {
        void  *addr;
        size_t size;
    };
}

Y_UTEST(alloc)
{
    random::cstdbits ran;
    memory::allocator &a = memory::global::instance();
    static const size_t N = 100;

    block blk[N];
    for(size_t i=0;i<N;++i)
    {
        blk[i].size = ran.leq(1000);
        blk[i].addr = a.acquire(blk[i].size);
    }

    ran.shuffle(blk,N);
    for(size_t i=0;i<N;++i)
    {
        a.release( blk[i].addr, blk[i].size );
    }
}
Y_UTEST_DONE()



