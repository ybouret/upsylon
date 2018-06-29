#include "y/memory/arena.hpp"
#include "y/utest/run.hpp"
#include "y/memory/cblock.hpp"

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

    const size_t nb = 4000;

    memory::cblock_of<block> blocks(nb);
    const size_t n   = blocks.size;
    const size_t h   = n/2;
    block       *blk = blocks.data;

    for(size_t block_size=1;block_size<=32;block_size+=1+alea.leq(5))
    {
        std::cerr << "block_size=" << block_size << std::endl;
        for(size_t chunk_size=1;chunk_size<=4096;chunk_size*=2)
        {
            //std::cerr << "\tchunk_size=" << chunk_size << std::endl;
            std::cerr << "/" << chunk_size;
            memory::arena A(block_size,chunk_size);

            for(size_t i=0;i<n;++i)
            {
                blk[i].addr = A.acquire();
            }
            alea.shuffle(blk,n);
            for(size_t i=0;i<h;++i)
            {
                A.release(blk[i].addr);
            }
            for(size_t i=0;i<h;++i)
            {
                blk[i].addr = A.acquire();
            }
            alea.shuffle(blk,n);
            for(size_t i=0;i<n;++i)
            {
                A.release(blk[i].addr);
            }
        }
        std::cerr << std::endl;
    }

    std::cerr << "sizeof(arena)=" << sizeof(memory::arena) << std::endl;

}
Y_UTEST_DONE()

