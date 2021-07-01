#include "y/sort/merge-pool.hpp"
#include "y/utest/run.hpp"
#include "y/core/node.hpp"

using namespace upsylon;

namespace upsylon
{
    typedef core::cpp_node_of<int>   iNode;
    typedef core::pool_of_cpp<iNode> iPool;


    static inline int compare_data( const iNode *lhs, const iNode *rhs, void *) throw()
    {
        return lhs->data-rhs->data;
    }

    static inline bool check_sum( const iPool &L ) throw()
    {
        size_t sum = 0;
        for(const iNode *node=L.head;node;node=node->next)
        {
            sum += node->data;
        }
        return ((L.size) * (L.size+1))>>1 == sum;
    }

    static inline bool check_org( const iPool &L ) throw()
    {
        int j=1;
        for(const iNode *node=L.head;node;node=node->next,++j)
        {
            if(node->data!=j) return false;
        }
        return true;
    }

#if 0
    static inline
    void display( const iPool &p )
    {
        std::cerr << "[";
        const iNode *node = p.head;
        if(node)
        {
            std::cerr << **node;
            while(NULL!=(node=node->next))
            {
                std::cerr << " " << **node;
            }
        }
        std::cerr << "]" << std::endl;
    }
#endif

}

Y_UTEST(sort_merge_pool)
{
    iPool L;
    for(size_t n=0;n<=2048;n+=16)
    {
        (std::cerr << '.').flush();
        // clear
        L.release();

        // fill 1..n
        for(size_t i=n;i>0;--i)
        {
            L.store( new iNode( int(i) ) );
        }
        Y_ASSERT(n==L.size);
        Y_ASSERT(check_org(L));

        for(size_t iter=0;iter<8;++iter)
        {
            // shuffle
            alea.shuffle_pool(L);
            Y_ASSERT(n==L.size);
            Y_ASSERT(check_sum(L));


            // sort to original
            merge_pool_of<iNode>::sort(L,compare_data,NULL);
            Y_ASSERT(n==L.size);
            Y_ASSERT(check_org(L));


            // shuffle
            alea.shuffle_pool(L);
            Y_ASSERT(n==L.size);
            Y_ASSERT(check_sum(L));

            // sort by increasing address
            merge_pool_of<iNode>:: by_increasing_address(L);
            Y_ASSERT(n==L.size);
            Y_ASSERT(check_sum(L));

            {
                const iNode *node = L.head;
                while(node&&node->next)
                {
                    Y_ASSERT( static_cast<ptrdiff_t>(node-node->next) < 0 );
                    node=node->next;
                }
            }



            // sort to original for another cycle
            merge_pool_of<iNode>::sort(L,compare_data,NULL);
            Y_ASSERT(n==L.size);
            Y_ASSERT(check_org(L));


        }


    }
    std::cerr << std::endl;
}
Y_UTEST_DONE()

