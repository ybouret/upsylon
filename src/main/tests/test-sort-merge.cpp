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

    static inline bool check_org( const iList &L ) throw()
    {
        int j=1;
        for(const iNode *node=L.head;node;node=node->next,++j)
        {
            if(node->data!=j) return false;
        }
        return true;
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
        Y_ASSERT(n==L.size);
        Y_ASSERT(check_org(L));


        for(size_t iter=0;iter<8;++iter)
        {
            // shuffle
            alea.shuffle(L);
            Y_ASSERT(n==L.size);
            Y_ASSERT(check_sum(L));

            // sort to original
            merging<iNode>::sort(L,compare_data,NULL);
            Y_ASSERT(n==L.size);
            Y_ASSERT(check_org(L));

            // shuffle
            alea.shuffle(L);
            Y_ASSERT(n==L.size);
            Y_ASSERT(check_sum(L));

            // sort by increasing address
            merging<iNode>::sort_by_increasing_address(L);
            Y_ASSERT(n==L.size);
            Y_ASSERT(check_sum(L));

            // sort to original for another cycle
            merging<iNode>::sort(L,compare_data,NULL);
            Y_ASSERT(n==L.size);
            Y_ASSERT(check_org(L));


        }


    }
    std::cerr << std::endl;


}
Y_UTEST_DONE()
