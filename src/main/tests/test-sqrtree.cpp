
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

        inline Node(const size_t x0,
                    const size_t x1) throw() :
        next(0), prev(0), skey(x0,x1)
        {

        }

        inline virtual ~Node() throw()
        {}

        Node *query( const size_t x0, const size_t x1 )
        {
            const Key k(x0,x1);
            for(Node *node=children.head;node;node=node->next)
            {
                if(k==node->skey) return node;
            }
            return children.push_back( new Node(x0,x1) );
        }

        inline void display( std::ostream &os, size_t level ) const
        {
            for(size_t i=level;i>0;--i) os << "  ";
            if(skey.x<=skey.y)
            {
                os << '[' << skey.x << ']' << std::endl;
            }
            else
            {
                os << '(' << skey.x << ',' << skey.y << ')' << std::endl;
            }
            ++level;
            for(const Node *node=children.head;node;node=node->next)
            {
                node->display(os,level);
            }

        }




    private:
        Y_DISABLE_COPY_AND_ASSIGN(Node);
    };

    template <typename T> static inline
    T __isqrt(const T n,
              Node   *root ) throw()
    {
        if(n<=1)
        {
            std::cerr << n << ':' << n  << std::endl;
            (void) root->query(n,n);
            return n;
        }
        else
        {
            Node *curr = root->query(n,n);
            T     x0   = n;
            T     x1   = (n>>1);
            std::cerr <<  x0;
            curr       = curr->query(x0,x1);
            while(true)
            {
                x0 = x1;
                x1 = ((x0+n/x0)>>1);
                curr   = curr->query(x0,x1);
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
    Node          *root = new Node(0,0);
    auto_ptr<Node> tree = root;

    for(size_t i=1;i<=16;++i)
    {
        (void)__isqrt(i,root);
    }

    root->display(std::cerr,0);

}
Y_UTEST_DONE()

