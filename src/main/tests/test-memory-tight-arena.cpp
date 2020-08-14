
#include "y/memory/tight/arena.hpp"
#include "y/memory/tight/quarry.hpp"
#include "y/memory/tight/zcache.hpp"
#include "y/memory/allocator/global.hpp"

#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"

#include "y/type/utils.hpp"
#include <iomanip>

using namespace upsylon;
using namespace memory;

Y_UTEST(tight_arena)
{
    const size_t count        = 1024;
    void        *entry[count] = {0 };

    tight::quarry Q;

    for(size_t chunk_size=32; chunk_size <= 8192; chunk_size <<= 1)
    {
        std::cerr << "<chunk_size=" << chunk_size << ">" << std::endl;
        tight::zcache<tight::chunk> cache(chunk_size,Q);
        std::cerr << "\tcache.chunk_size  = " << cache.chunk_size << std::endl;
        std::cerr << "\tcache.nodes_rise  = " << cache.nodes_rise << std::endl;
        const size_t block_max = min_of<size_t>(chunk_size/2,60);
        for(size_t block_size=1;block_size<=block_max;++block_size)
        {
            tight::arena t(block_size,chunk_size,&cache,Q);
            std::cerr << "\t<block_size=" << std::setw(3) << block_size << ">";
            std::cerr << " chunk_size=" << std::setw(5) << t.chunk_size;
            std::cerr << " blocks/chunk=" << std::setw(3) << t.blocks_per_chunk;
            std::cerr << std::endl;

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

            std::cerr << t << std::endl;

            for(size_t i=0;i<count;++i)
            {
                t.release(entry[i]);
            }

        }
        std::cerr << "<chunk_size=" << chunk_size << "/>" << std::endl;
        std::cerr << std::endl;
    }

    std::cerr << Q << std::endl;

    Y_UTEST_SIZEOF(tight::chunk);
    Y_UTEST_SIZEOF(tight::arena);

}
Y_UTEST_DONE()

#include "y/string/convert.hpp"
#include "y/sort/heap.hpp"

Y_UTEST(tight_compact)
{
    concurrent::singleton::verbose = true;
    
    size_t     chunk_size = 1024;
    if(argc>1) chunk_size = string_convert::to<size_t>(argv[1],"chunk_size");

    tight::quarry               Q;
    tight::zcache<tight::chunk> cache(chunk_size,Q);
    const size_t count        = 1024;
    void        *entry[count] = { 0 };

    for(size_t block_size=1;block_size<=40;++block_size)
    {
        std::cerr << "block_size=" << block_size << std::endl;
        tight::arena a(block_size,chunk_size,&cache,Q);

        size_t n = 0;
        while(n<count)
        {
            entry[n++] = a.acquire();
        }
        alea.shuffle(entry,n);
        while(n>count/2)
        {
            a.release(entry[--n]);
        }
        hsort(entry,n, comparison::increasing<void*> );
        size_t ok = 0;
        for(size_t i=0;i<n;++i)
        {
            if( a.compact(entry[i]) )
            {
                std::cerr << '+';
                ++ok;
            }
            else
            {
                std::cerr << '-';
            }
        }
        std::cerr << std::endl;
        while(n>0)
        {
            a.release(entry[--n]);
        }
        std::cerr << Q << std::endl;
    }



}
Y_UTEST_DONE()
