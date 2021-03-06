

#include "y/memory/tight/blocks.hpp"
#include "y/memory/tight/quarry.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/memory/allocator/global.hpp"
#include "y/type/utils.hpp"

#include <iomanip>

using namespace upsylon;
using namespace memory;


namespace {

    struct blk_t
    {
        void  *addr;
        size_t size;
    };

}

Y_UTEST(tight_blocks)
{
    concurrent::singleton::verbose = true;

    tight::quarry Q;
    for(size_t chunk_size=1; chunk_size<=8192; chunk_size<<=1 )
    {
        std::cerr << "<chunk_size=" << chunk_size << ">" << std::endl;
        const size_t limit_max = min_of<size_t>(chunk_size/2,512);
        for(size_t limit_size=1;limit_size<=limit_max;limit_size<<=1)
        {
            std::cerr << "\tlimit_size=" << std::setw(5) << limit_size << ":";

            tight::blocks blk(chunk_size,limit_size,Q);
            std::cerr << " | chunk_size=" << std::setw(5) <<  blk.chunk_size;
            std::cerr << " | #slots="     << std::setw(3) <<  blk.slots_size;
            std::cerr << " | load="       << std::setw(3) <<  blk.load_factor();
            //std::cerr << " | chunks#size,rise="  << std::setw(5) <<  blk.zChunks.chunk_size << "," << std::setw(2) <<  blk.zChunks.nodes_rise;
            //std::cerr << " | arenas#size,rise="  << std::setw(5) <<  blk.zArenas.chunk_size << "," << std::setw(2) <<  blk.zArenas.nodes_rise;
            std::cerr << std::endl;
            

            const size_t max_size = limit_size+1;
            const size_t bps      = 500;

            const size_t nblk = bps*max_size;
            blk_t       *blks = static_cast<blk_t *>( global::instance().__calloc(nblk,sizeof(blk_t)) );


            // create blocks
            {
                size_t j=0;
                for(size_t i=1;i<=limit_size;++i)
                {
                    for(size_t k=bps;k>0;--k)
                    {
                        blk_t &b = blks[j++];
                        b.addr = 0;
                        b.size = i;
                    }
                }

                for(size_t k=bps;k>0;--k)
                {
                    blk_t &b = blks[j++];
                    b.addr = 0;
                    b.size = alea.range(limit_size+1, 2*limit_size);
                }


                Y_ASSERT(nblk==j);
            }

            // allocate blocks
            alea.shuffle(blks,nblk);
            for(size_t i=0;i<nblk;++i)
            {
                blk_t &b = blks[i];
                b.addr   = blk.acquire(b.size);
            }

            //std::cerr << blk << std::endl;

            // deallocate
            alea.shuffle(blks,nblk);
            for(size_t i=0;i<nblk;++i)
            {
                blk_t &b = blks[i];
                blk.release(b.addr,b.size);
            }
            global::location().__free(blks,nblk*sizeof(blk_t));
            
        }
        std::cerr << "<chunk_size=" << chunk_size << "/>" << std::endl;
        std::cerr << Q << std::endl;
        std::cerr << std::endl;
    }
    Y_UTEST_SIZEOF(tight::blocks::slot_type);


}
Y_UTEST_DONE()

