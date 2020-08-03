
#include "y/memory/small/pages.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include <iomanip>

using namespace upsylon;
using namespace memory;

Y_UTEST(pages)
{

    for(size_t chunk_size=1;chunk_size<=8192;chunk_size<<=1)
    {
        small::pages cache(chunk_size);
        std::cerr
        << std::setw(5) << chunk_size << " => "
        << std::setw(5) << cache.chunk_size << " => "
        << std::setw(5) << cache.chunks_per_page << " pieces/page"
        << std::endl;

        const size_t n = cache.chunks_per_page * 5;
        core::list_of<small::chunk> chunks;
        for(size_t i=0;i<n;++i)
        {
            chunks.push_back( cache.query_nil() );
        }
        //std::cerr << "cache.zstore=" << cache.zstore.size << std::endl;
       // std::cerr << "cache.pieces=" << cache.pieces.size << std::endl;
        std::cerr << "chunks      =" << chunks.size << std::endl;
        alea.shuffle(chunks);

        while(chunks.size)
        {
            cache.store_nil( chunks.pop_back() );
        }

    }

    Y_UTEST_SIZEOF(small::chunk);
    Y_UTEST_SIZEOF(small::pages);

}
Y_UTEST_DONE()
