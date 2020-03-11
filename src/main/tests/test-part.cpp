#include "y/counting/part.hpp"
#include "y/utest/run.hpp"
#include "y/core/node.hpp"
#include "y/core/list.hpp"

using namespace upsylon;

namespace {



    typedef core::cpp_node_of<unsigned>  node_type;
    typedef core::list_of_cpp<node_type> list_type;

    inline void display(const list_type &L)
    {
        std::cerr << "[";
        for(const node_type *node=L.head;node;node=node->next)
        {
            std::cerr << " " << node->data;
        }
        std::cerr << " ]" << std::endl;
    }

    inline void next_list(list_type       &b,
                          const list_type &a )
    {
        assert(0==b.size);
        unsigned N = 0;
        unsigned k = 0;
        {
            size_t i = 0;
            for(const node_type *node=a.head;node;node=node->next)
            {
                assert(node->data>0);
                ++i;
                if(node->data<=1)
                {
                    ++N;
                }
                else
                {
                    k=i;
                }
            }
            assert(i==a.size);
        }

        {
            const node_type *node = a.head;
            for(size_t i=1;i<k;++i,node=node->next)
            {
                assert(node);
                b << node->data;
            }
            const unsigned bk = node->data-1;
            div_t d = div( int(N+1), int(bk) );
            const unsigned q = d.quot;
            const unsigned r = d.rem;

            for(unsigned j=k;j<=k+q;++j)
            {
                b << bk;
            }
            if(r>0)
            {
                b << r;
            }

        }


    }


}

#include "y/string/convert.hpp"

Y_UTEST(part)
{
    unsigned n = 5;
    if( argc > 1 )
    {
        n = string_convert::to<size_t>( argv[1], "n" );
    }
    if(n<=1) throw exception("n<=1");
    list_type L;
    L.push_back( new node_type(n) );

    display(L);
    size_t count = 1;
    while( L.size < n )
    {
        list_type L2;
        next_list(L2,L);

        L.swap_with(L2);
        display(L);
        ++count;
    }

    std::cerr << "#count=" << count << std::endl;

}
Y_UTEST_DONE()

