
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
        << std::setw(5) << cache.pieces_per_page << " pieces/page"
        << std::endl;

        const size_t n = cache.pieces_per_page * 5;
        core::list_of<small::piece> pieces;
        for(size_t i=0;i<n;++i)
        {
            pieces.push_back( cache.query_nil() );
        }
        //std::cerr << "cache.zstore=" << cache.zstore.size << std::endl;
       // std::cerr << "cache.pieces=" << cache.pieces.size << std::endl;
        std::cerr << "pieces      =" << pieces.size << std::endl;
        alea.shuffle(pieces);

        while(pieces.size)
        {
            cache.store_nil( pieces.pop_back() );
        }

    }

    Y_UTEST_SIZEOF(small::piece);
    Y_UTEST_SIZEOF(small::pages);

}
Y_UTEST_DONE()
