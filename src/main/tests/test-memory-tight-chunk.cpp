#include "y/memory/tight/chunk.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/memory/allocator/global.hpp"
#include "y/type/utils.hpp"
#include <iomanip>

using namespace upsylon;
using namespace memory;

Y_UTEST(tight_chunk)
{

    global &mgr = global::instance();

    void *addr[256] = {};

    for(size_t block_size=1;block_size<=40;++block_size)
    {
        std::cerr << "<block_size=" << block_size << ">" << std::endl;
        const size_t min_chunk_size =  tight::chunk::min_chunk_size_for(block_size);
        const size_t max_chunk_size =  tight::chunk::max_chunk_size_for(block_size);
        std::cerr << "\tmin_chunk_size=" << min_chunk_size << std::endl;
        std::cerr << "\tmax_chunk_size=" << max_chunk_size << std::endl;

        for(size_t chunk_size=min_chunk_size;chunk_size<=max_chunk_size;chunk_size<<=1)
        {
            void *chunk_data = mgr.__calloc(1,chunk_size);

            tight::chunk p(block_size,chunk_data,chunk_size);
            std::cerr << "\tchunk_size=" << std::setw(5) << chunk_size << " " << p << "[";

            for(size_t iter=0;iter<256;++iter)
            {
                if( 0==((iter+1)%16)) std::cerr << ".";

                while(p.still_available)
                {
                    void * &q = addr[p.allocated()];
                    q = p.acquire(block_size);
                }
                Y_ASSERT(0==p.still_available);
                alea.shuffle(addr,p.provided_number);
                while( !p.is_empty() )
                {
                    void * &q = addr[p.allocated()-1];
                    p.release(q,block_size);
                }
                Y_ASSERT(p.is_empty());
            }


            std::cerr << "]";
            while(p.still_available)
            {
                (void) p.acquire(block_size);
            }
            std::cerr << p;
            std::cerr << std::endl;
            mgr.__free(chunk_data,chunk_size);
        }
        



        std::cerr << "<block_size=" << block_size << "/>" << std::endl << std::endl;
    }

    if(false)
    {
        for(size_t block_size=1;block_size<=40;++block_size)
        {
            const size_t mcs = tight::chunk::max_chunk_size_for(block_size);
            const size_t bpc = min_of<size_t>(255,mcs/block_size);
            std::cerr
            << std::setw(3) << tight::chunk::min_chunk_size_for(block_size) << " : "
            << std::setw(3) << block_size << " : " << std::setw(6) << mcs << " => " << std::setw(3) << bpc << " blocks/chunk" << std::endl;
        }
    }


}
Y_UTEST_DONE()


