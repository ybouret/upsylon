
#include "y/memory/tight/twain-allocator.hpp"
#include "y/utest/run.hpp"

#include "y/concurrent/fake-lock.hpp"
#include "y/string/convert.hpp"

using namespace upsylon;
using namespace memory;

namespace {

    struct tblock
    {
        void  *addr;
        size_t size;
        size_t exp2;
    };

}

Y_UTEST(tight_twain)
{

    size_t chunk_size = 4096;
    size_t limit_size = 128;

    if(argc>1)
    {
        chunk_size = string_convert::to<size_t>(argv[1],"chunk_size");
    }

    if(argc>2)
    {
        limit_size = string_convert::to<size_t>(argv[2],"limit_size");
    }


    concurrent::fake_lock sync;
    tight::quarry         Q;


    tight::twain_allocator twa(sync,Q,chunk_size,16,limit_size);

    std::cerr << "sizeof(arena)=" << sizeof(tight::arena) << std::endl;
    std::cerr << "sizeof(twain_allocator)=" << sizeof(tight::twain_allocator) << std::endl;

    tblock blk[1024];
    const size_t num = sizeof(blk)/sizeof(blk[0]);
    memset(blk,0,sizeof(blk));
    for(size_t i=0;i<num/2;++i)
    {
        tblock &b = blk[i];
        b.size = i;
        b.exp2 = 0;
        b.addr = twa.acquire(b.size,b.exp2);
        Y_ASSERT(b.addr);
    }

    for(size_t i=num/2;i<num;++i)
    {
        tblock &b = blk[i];
        b.size = alea.leq( 5 * limit_size );
        b.exp2 = 0;
        b.addr = twa.acquire(b.size,b.exp2);
        Y_ASSERT(b.addr);
    }

    alea.shuffle(blk,num);

    for(size_t i=0;i<num/2;++i)
    {
        twa.release(blk[i].addr, blk[i].size, blk[i].exp2);
    }




}
Y_UTEST_DONE()

