#include "y/memory/blocks.hpp"
#include "y/utest/run.hpp"
#include "y/type/utils.hpp"
#include "y/memory/cblock.hpp"
#include <cstdlib>

using namespace upsylon;

namespace  {
    struct block
    {
        size_t size;
        void  *addr;
    };
}
Y_UTEST(blocks)
{


    for(size_t chunk_size=1;chunk_size<=4096;chunk_size*=2)
    {
        memory::blocks B(chunk_size);


        const size_t n = 1000;
        const size_t h = n/2;
        memory::cblock_of<block> wksp(n);
        block *blk = wksp.data;
        std::cerr << "#pages,arenas:";
        for(size_t iter=0;iter<16;++iter)
        {
            for(size_t i=0;i<n;++i)
            {
                blk[i].size = 1+alea.leq(100);
                blk[i].addr = B.acquire(blk[i].size);
            }

            alea.shuffle(blk,n);
            for(size_t i=0;i<h;++i)
            {
                B.release(blk[i].addr,blk[i].size);
            }

            for(size_t i=0;i<h;++i)
            {
                blk[i].size = 1+alea.leq(100);
                blk[i].addr = B.acquire(blk[i].size);
            }
            alea.shuffle(blk,n);
            for(size_t i=0;i<n;++i)
            {
                Y_ASSERT( B.owns(blk[i].addr,blk[i].size) );
                B.release(blk[i].addr,blk[i].size);
            }
            std::cerr << B.num_pages() << "," << B.num_arenas() << "/";
        }
        std::cerr << std::endl;
    }


}
Y_UTEST_DONE()

