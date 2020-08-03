#include "y/memory/small/piece.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/memory/allocator/global.hpp"
//#include "y/type/utils.hpp"
#include <iomanip>

using namespace upsylon;
using namespace memory;

Y_UTEST(piece)
{

    global &mgr = global::instance();

    void *addr[256] = {};

    for(size_t block_size=1;block_size<=40;++block_size)
    {
        std::cerr << "<block_size=" << block_size << ">" << std::endl;
        const size_t min_chunk_size =  small::piece::min_chunk_size_for(block_size);
        const size_t max_chunk_size =  small::piece::max_chunk_size_for(block_size);
        std::cerr << "\tmin_chunk_size=" << min_chunk_size << std::endl;
        std::cerr << "\tmax_chunk_size=" << max_chunk_size << std::endl;

        for(size_t chunk_size=min_chunk_size;chunk_size<=max_chunk_size;chunk_size<<=1)
        {
            void *chunk_data = mgr.__calloc(1,chunk_size);

            small::piece p(block_size,chunk_data,chunk_size);
            std::cerr
            << "\t\tchunk_size=" << std::setw(5) << chunk_size
            << " : #blocks="     << std::setw(3) << int(p.provided_number)
            << " in " << std::setw(5) << (p.last-p.data) << " bytes [";

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
            }

            std::cerr << "]" << std::endl;
            mgr.__free(chunk_data,chunk_size);
        }




        std::cerr << "<block_size=" << block_size << "/>" << std::endl << std::endl;
    }

    Y_UTEST_SIZEOF(small::piece);

}
Y_UTEST_DONE()


