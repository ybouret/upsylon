#include "y/memory/blocks.hpp"
#include "y/utest/run.hpp"
#include "y/type/utils.hpp"
#include "y/ptr/cblock.hpp"
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
        std::cerr << "requested chunk_size: " << chunk_size     << std::endl;
        std::cerr << "allocated chunk_size: " << B.chunk_size   << std::endl;
        std::cerr << "computed  table_mask: " << B.table_mask   << std::endl;
        std::cerr << "computed  table_size: " << B.table_mask+1 << std::endl;

        const size_t n = 1000;
        const size_t h = n/2;
        cblock<block> wksp(n);
        block *blk  = &wksp[0];
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
        }
        std::cerr << std::endl;
    }


}
Y_UTEST_DONE()

