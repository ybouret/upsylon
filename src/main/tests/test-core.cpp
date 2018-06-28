#include "y/core/list.hpp"
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
        LIST l_copy(l);
        Y_CHECK(l.size==l_copy.size);
    }

}



Y_UTEST(core)
{
    core::list_of_cpp<aNode> aList;
    do_list_test(aList);
    core::list_of_cloneable<aNode> bList;
    do_list_test(bList);
}
Y_UTEST_DONE()

