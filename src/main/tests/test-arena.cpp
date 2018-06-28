#include "y/memory/arena.hpp"
#include "y/utest/run.hpp"
#include "y/memory/global.hpp"

using namespace upsylon;
namespace
{
    struct block
    {
        void *addr;
    };
}

Y_UTEST(arena)
{
    memory::global &hmem = memory::global::instance();
    memory::arena A(1,1024);

    size_t blk_count = 1000;
    size_t blk_bytes = 0;
    block *blk = hmem.acquire<block>(blk_count,blk_bytes);
    std::cerr << "#blk_count=" << blk_count << std::endl;
    std::cerr << "#blk_bytes=" << blk_bytes << std::endl;


    const size_t hblk = blk_count/2;
    for(size_t iter=0;iter<16;++iter)
    {
        for(size_t i=0;i<blk_count;++i)
        {
            blk[i].addr = A.acquire();
        }

        alea.shuffle(blk,blk_count);
        for(size_t i=0;i<hblk;++i)
        {
            A.release(blk[i].addr);
        }
        for(size_t i=0;i<hblk;++i)
        {
            blk[i].addr = A.acquire();
        }
        alea.shuffle(blk,blk_count);
        for(size_t i=0;i<blk_count;++i)
        {
            A.release(blk[i].addr);
        }
    }

    hmem.release(blk,blk_count,blk_bytes);
    
}
Y_UTEST_DONE()

