
#include "y/memory/small/zcache.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include <iomanip>

using namespace upsylon;
using namespace memory;

namespace {

    template <size_t N>
    struct Node
    {
        Node *next;
        Node *prev;
        char  data[N];
    };

    template <size_t N>
    static inline void doTestZ( small::quarry &Q )
    {
        typedef Node<N> node_type;
        std::cerr << "sizeof(Node)=" << sizeof(node_type) << std::endl;
        for(size_t chunk_size=1;chunk_size<=8192;chunk_size<<=1)
        {
            small::zcache<node_type> cache(chunk_size,Q);
            std::cerr << "\t"
            << std::setw(5) << chunk_size << " => "
            << std::setw(5) << cache.chunk_size << " => "
            << std::setw(5) << cache.nodes_rise << " nodes/alloc...";

            const size_t n = cache.nodes_rise * 5;
            core::list_of<node_type> chunks;
            for(size_t i=0;i<n;++i)
            {
                chunks.push_back( cache.query_nil() );
            }
            //std::cerr << "cache.zstore=" << cache.zstore.size << std::endl;
            // std::cerr << "cache.pieces=" << cache.pieces.size << std::endl;
            std::cerr << "nodes = " << std::setw(5) << chunks.size << std::endl;
            alea.shuffle(chunks);

            while(chunks.size)
            {
                cache.store_nil( chunks.pop_back() );
            }

        }
    }
}

Y_UTEST(small_zcache)
{
    small::quarry Q;
    doTestZ<1>(Q);
    doTestZ<10>(Q);
    doTestZ<100>(Q);


    Y_UTEST_SIZEOF( small::zcache< Node<1>     >);
    Y_UTEST_SIZEOF( small::zcache< Node<10>  >);
    Y_UTEST_SIZEOF( small::zcache< Node<100> >);



}
Y_UTEST_DONE()
