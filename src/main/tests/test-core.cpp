#include "y/core/list.hpp"
#include "y/core/pool.hpp"
#include "y/core/loop.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

namespace
{
    class aNode
    {
    public:
        int    data;
        aNode *next;
        aNode *prev;

        aNode() throw() : data( alea.full<int>() ), next(0), prev(0) {}
        ~aNode() throw() {}

        aNode( const aNode &other ) throw() : data( other.data ), next(0), prev(0) {}

        inline aNode * clone() const { return new aNode( *this ); }


    private:
        Y_DISABLE_ASSIGN(aNode);
    };

    template <typename LIST>
    void do_list_test( LIST &l )
    {
        const size_t n = 10 + alea.leq(1000);
        for(size_t i=0;i<n;++i)
        {
            aNode *node = new aNode();
            if( alea.choice() )
            {
                std::cerr << ">";
                l.push_back(node);
            }
            else
            {
                std::cerr << "<";
                l.push_front(node);
            }
        }
        std::cerr << std::endl;
        Y_CHECK(l.size==n);
        {
            LIST l_copy(l);
            Y_CHECK(l.size==l_copy.size);
            l.merge_back(l_copy);
        }



        core::pool_of_cpp<aNode> p;
        while( l.size ) p.store( l.pop_back() );

        std::cerr << "MoveToFront 1->" << n << std::endl;
        for(size_t i=0;i<n;++i)
        {
            l.push_back( new aNode() );
            for(size_t j=0;j<l.size;++j)
            {
                aNode *node = l.fetch(j);
                if(l.index_of(node)!=j)
                {
                    throw exception("invalid fetch/index");
                }
                l.move_to_front( l.fetch(j) );
            }
        }

    }


    static inline
    void do_loop_test()
    {
        int a[256];

        for(size_t iter=0;iter<10;++iter)
        {
            const size_t n = 1+alea.leq(1000);
            std::cerr << "...loop " << n << std::endl;
            size_t count=0;
            Y_LOOP(0,++count);
            Y_LOOP(n,++count);
            Y_CHECK(n==count);
            count=0;
            Y_LOOP_(n,++count);
            Y_CHECK(n==count);
#define THE_CODE(IDX) a[IDX] = alea.full<int>()
            Y_LOOP_FUNC( sizeof(a)/sizeof(a[0]),THE_CODE,0);
            Y_LOOP_FUNC_(sizeof(a)/sizeof(a[0]),THE_CODE,0);
            Y_LOOP_FUNC( sizeof(a)/sizeof(a[0])-1,THE_CODE,1);
            Y_LOOP_FUNC_(sizeof(a)/sizeof(a[0])-1,THE_CODE,1);
        }
    }
}



Y_UTEST(core)
{
    std::cerr << "---- Test Loop ----" << std::endl;
    do_loop_test();

    std::cerr << "---- Test List ----" << std::endl;
    std::cerr << "sizeof(core::list_of)=" << sizeof(core::list_of<aNode>) << std::endl;
    core::list_of_cpp<aNode> aList;
    do_list_test(aList);
    core::list_of_cloneable<aNode> bList;
    do_list_test(bList);
}
Y_UTEST_DONE()

