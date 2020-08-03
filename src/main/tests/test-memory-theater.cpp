
#include "y/memory/small/theater.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/memory/allocator/global.hpp"
#include <iomanip>

using namespace upsylon;
using namespace memory;

Y_UTEST(theater)
{

#if 0
    const size_t usr_block_size = 8000;
    for(size_t block_size=1;block_size<=60;++block_size)
    {
        small::theater t(block_size,usr_block_size);
        Y_ASSERT(t.block_size==block_size);
        std::cerr << "theater<" << std::setw(3) << t.block_size << ">"
        << " : chunk_size=" << std::setw(5) << t.chunk_size
        << " : page_bytes=" << std::setw(5) << t.page_bytes
        << " : pieces_per_page=" << std::setw(5) << t.ppp
        << std::endl;

        t.query_piece();
        
    }
#endif

    for(size_t chunk_size=1;chunk_size<=8192;chunk_size<<=1)
    {
        small::pages cache(chunk_size);
        std::cerr
        << std::setw(5) << chunk_size << " => "
        << std::setw(5) << cache.chunk_size << " => "
        << std::setw(5) << cache.pieces_per_page << " pieces/page"
        << std::endl;
    }

    Y_UTEST_SIZEOF(small::piece);
    Y_UTEST_SIZEOF(small::pages);
    Y_UTEST_SIZEOF(small::theater);

}
Y_UTEST_DONE()
