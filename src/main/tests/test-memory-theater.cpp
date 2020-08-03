
#include "y/memory/small/theater.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/memory/allocator/global.hpp"
#include <iomanip>

using namespace upsylon;
using namespace memory;

Y_UTEST(theater)
{
    const size_t count        = 1024;
    void        *entry[count] = {0 };

    for(size_t chunk_size=1; chunk_size <= 8192; chunk_size <<= 1)
    {
        std::cerr << "<chunk_size=" << chunk_size << ">" << std::endl;
        small::pages cache(chunk_size);
        std::cerr << "\tcache.chunk_size=" << cache.chunk_size << std::endl;
        
        for(size_t block_size=1;block_size<=60;++block_size)
        {
            small::theater t(block_size,chunk_size,cache);
            std::cerr << "\t<block_size=" << std::setw(3) << block_size << ">";
            std::cerr << " chunk_size=" << std::setw(5) << t.chunk_size;

            size_t n = 0;
            for(size_t i=0;i<count;++i)
            {
                entry[n++] = t.acquire();
            }
            alea.shuffle(entry,count);


            for(size_t i=0;i<count;++i)
            {
                t.release(entry[i]);
            }

            std::cerr << std::endl;
        }
        std::cerr << "<chunk_size=" << chunk_size << "/>" << std::endl;
        std::cerr << std::endl;
    }


    Y_UTEST_SIZEOF(small::piece);
    Y_UTEST_SIZEOF(small::pages);
    Y_UTEST_SIZEOF(small::theater);

}
Y_UTEST_DONE()
