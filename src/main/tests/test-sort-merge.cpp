#include "y/sort/merge.hpp"
#include "y/utest/run.hpp"
#include "y/core/node.hpp"

using namespace upsylon;

namespace upsylon
{
    typedef core::cpp_node_of<int>   iNode;
    typedef core::list_of_cpp<iNode> iList;


    static inline int compare_data( const iNode *lhs, const iNode *rhs, void *) throw()
    {
        return lhs->data-rhs->data;
    }

    static inline bool check_sum( const iList &L ) throw()
    {
        size_t sum = 0;
        for(const iNode *node=L.head;node;node=node->next)
        {
            sum += node->data;
        }
        return ((L.size) * (L.size+1))>>1 == sum;
    }
}

Y_UTEST(sort_merge)
{

    iList L;
    for(size_t n=0;n<=512;++n)
    {
        (std::cerr << '.').flush();
        // clear
        L.release();

        // fill 1..n
        for(size_t i=1;i<=n;++i)
        {
            L.push_back( new iNode( int(i) ) );
        }

        for(size_t iter=0;iter<8;++iter)
        {
            // shuffle
            Y_ASSERT(n==L.size); alea.shuffle(L); Y_ASSERT(n==L.size);

            // check sum :)
            Y_ASSERT(check_sum(L));

            merging<iNode>::sort(L,compare_data,NULL);
            {
                int j=1;
                for(const iNode *node=L.head;node;node=node->next,++j)
                {
                    Y_ASSERT(node->data==j);
                }
            }
        }


    }
    std::cerr << std::endl;


}
Y_UTEST_DONE()

