
#include "y/memory/small/theater.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/memory/allocator/global.hpp"
#include "y/type/utils.hpp"
#include <iomanip>

using namespace upsylon;
using namespace memory;

Y_UTEST(theater)
{
    const size_t count        = 1024;
    void        *entry[count] = {0 };

    for(size_t chunk_size=32; chunk_size <= 8192; chunk_size <<= 1)
    {
        std::cerr << "<chunk_size=" << chunk_size << ">" << std::endl;
        small::pages cache(chunk_size);
        std::cerr << "\tcache.chunk_size      = " << cache.chunk_size << std::endl;
        std::cerr << "\tcache.chunks_per_page = " << cache.chunks_per_page << std::endl;
        const size_t block_max = min_of<size_t>(chunk_size,60);
        for(size_t block_size=1;block_size<=block_max;++block_size)
        {
            small::theater t(block_size,chunk_size,cache);
            std::cerr << "\t<block_size=" << std::setw(3) << block_size << ">";
            std::cerr << " chunk_size=" << std::setw(5) << t.chunk_size;
            std::cerr << " blocks_per_piece=" << std::setw(3) << t.blocks_per_piece();
            std::cerr << std::endl;

#if !defined(NDEBUG)
            if(t.blocks_per_piece()<=1) continue;
#endif
            
            size_t n = 0;
            while(n<count)
            {
                entry[n++] = t.acquire();
            }
            alea.shuffle(entry,count);

            while(n>count/2)
            {
                t.release( entry[--n] );
            }

            while(n<count)
            {
                entry[n++] = t.acquire();
            }
            alea.shuffle(entry,count);


            for(size_t i=0;i<count;++i)
            {
                t.release(entry[i]);
            }

        }
        std::cerr << "<chunk_size=" << chunk_size << "/>" << std::endl;
        std::cerr << std::endl;
    }


    Y_UTEST_SIZEOF(small::chunk);
    Y_UTEST_SIZEOF(small::pages);
    Y_UTEST_SIZEOF(small::theater);

}
Y_UTEST_DONE()
