

#include "y/memory/small/blocks.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/memory/allocator/global.hpp"

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

Y_UTEST(small_blocks)
{

    for(size_t chunk_size=1; chunk_size<=8192; chunk_size<<=1 )
    {
        std::cerr << "<chunk_size=" << chunk_size << ">" << std::endl;
        for(size_t limit_size=1;limit_size<=512;limit_size<<=1)
        {
            std::cerr << "\tlimit_size=" << std::setw(5) << limit_size << ":";

            small::blocks blk(chunk_size,limit_size);
            std::cerr << " | chunk_size="        << std::setw(5) <<  blk.chunk_size;
            std::cerr << " | slots_size="        << std::setw(5) <<  blk.slots_size;
            std::cerr << " | load_factor="       << std::setw(3) <<  blk.load_factor();
            std::cerr << " | chunks#size,rise="  << std::setw(5) <<  blk.chunks.chunk_size << "," << std::setw(5) <<  blk.chunks.nodes_rise;
            std::cerr << " | arenas#size,rise="  << std::setw(5) <<  blk.arenas.chunk_size << "," << std::setw(5) <<  blk.arenas.nodes_rise;
            std::cerr << std::endl;

            size_t nblk = 1000;
            blk_t *blks = static_cast<blk_t *>( global::instance().__calloc(nblk,sizeof(blk_t)) );

            for(size_t iter=0;iter<16;++iter)
            {
                for(size_t i=0;i<nblk;++i)
                {
                    blk_t &b = blks[i];
                    b.size   = alea.range<size_t>(0,2*limit_size);
                    b.addr   = blk.acquire(b.size);
                }
                alea.shuffle(blks,nblk);

                for(size_t i=0;i<nblk;++i)
                {
                    blk_t &b = blks[i];
                    blk.release(b.addr,b.size);
                }
            }


            global::location().__free(blks,nblk*sizeof(blk_t));


        }
        std::cerr << "<chunk_size=" << chunk_size << "/>" << std::endl;
        std::cerr << std::endl;
    }
    
    Y_UTEST_SIZEOF(small::blocks::slot_type);


}
Y_UTEST_DONE()

