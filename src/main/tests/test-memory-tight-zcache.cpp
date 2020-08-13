
#include "y/memory/tight/zcache.hpp"
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
    static inline void doTestZ( tight::quarry &Q )
    {
        typedef Node<N> node_type;
        std::cerr << "sizeof(Node)=" << sizeof(node_type) << std::endl;
        for(size_t chunk_size=1;chunk_size<=8192;chunk_size<<=1)
        {
            tight::zcache<node_type> cache(chunk_size,Q);
            std::cerr << "\tchunk_size "
            << std::setw(5) << chunk_size << " => "
            << std::setw(5) << cache.chunk_size << " => "
            << std::setw(5) << cache.nodes_rise << " nodes/alloc...";

            const size_t n = cache.nodes_rise * (2+alea.leq(10));
            core::list_of<node_type> nodes;
            for(size_t i=0;i<n;++i)
            {
                nodes.push_back( cache.zquery() );
            }
            std::cerr << "nodes = " << std::setw(5) << nodes.size << std::endl;
            alea.shuffle(nodes);

            const size_t h=nodes.size/2;

            while(nodes.size>h)
            {
                cache.zstore( nodes.pop_back() );
            }

            cache.gc();

            while(nodes.size)
            {
                cache.zstore( nodes.pop_back() );
            }

            cache.gc();
        }
        std::cerr << Q << std::endl;
    }
}

Y_UTEST(tight_zcache)
{
    tight::quarry Q;
    doTestZ<1>(Q);
    doTestZ<10>(Q);
    doTestZ<100>(Q);


    Y_UTEST_SIZEOF( tight::zcache< Node<1>     >);
    Y_UTEST_SIZEOF( tight::zcache< Node<10>  >);
    Y_UTEST_SIZEOF( tight::zcache< Node<100> >);



}
Y_UTEST_DONE()
