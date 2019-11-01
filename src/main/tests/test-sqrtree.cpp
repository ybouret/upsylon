
#include "y/core/isqrt.hpp"
#include "y/utest/run.hpp"
#include "y/core/list.hpp"
#include "y/object.hpp"
#include "y/type/point2d.hpp"
#include "y/ptr/auto.hpp"
#include <cstdio>

using namespace upsylon;

namespace {

    typedef point2d<size_t> Key;

    class Node;
    typedef core::list_of_cpp<Node> List;

    class Node : public object
    {
    public:
        Node     *next;
        Node     *prev;
        const Key skey;
        List      children;

        inline Node(const size_t x0, const size_t x1) throw() :
        next(0), prev(0), skey(x0,x1)
        {

        }

        inline virtual ~Node() throw()
        {}

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Node);
    };

    template <typename T> static inline
    T __isqrt(const T n,
              List   &top ) throw()
    {
        if(n<=1)
        {
            std::cerr << n << ':' << n  << std::endl;
            top.push_back( new Node(n,n) );
            return n;
        }
        else
        {
            top.push_back( new Node(n,n) );

            T x0 = n;
            T x1 = (n>>1);
            std::cerr <<  x0;
            //const Key skey(x0,x1);

            while(true)
            {
                x0 = x1;
                x1 = ((x0+n/x0)>>1);
                if(x1>=x0)
                {
                    std::cerr << ':' << x0  << std::endl;
                    return x0;
                }
                else
                {
                    std::cerr << '(' << x0 << ',' << x1 << ')';
                }
            }
        }
    }
}


Y_UTEST(sqrtree)
{

    List top;
    for(size_t i=1;i<=100;++i)
    {
        (void)__isqrt(i,top);
    }

}
Y_UTEST_DONE()

