#include "y/core/node.hpp"
#include "y/core/list.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include "y/comparison.hpp"
#include "y/sort/merge.hpp"
#include "y/sort/heap.hpp"
#include "y/sequence/vector.hpp"

using namespace upsylon;

namespace
{


    template <typename NODE>
    int compare_node( const NODE *lhs, const NODE *rhs, void *)
    {
        return comparison::increasing(lhs->data,rhs->data);
    }

    template <typename T> static inline
    void perform()
    {
        typedef core::node_of<T> Node;
        core::list_of_cpp<Node>  L;
        for(size_t i=1;i<=10;++i)
        {
            const T args = support::get<T>();
            L.push_back( new Node(args) );
        }
        std::cerr << "L1=";
        for(const Node *node=L.head;node;node=node->next)
        {
            std::cerr << node->data << " ";
        }
        std::cerr << std::endl;
        merging<Node>::sort(L,compare_node<Node>,NULL);
        std::cerr << "L2=" ;
        for(const Node *node=L.head;node;node=node->next)
        {
            std::cerr << node->data << " ";
        }
        std::cerr << std::endl;

        vector<T> V;
        while(L.size)
        {
            if( alea.choice() )
            {
                V.push_back(L.head->data);
                delete L.pop_front();
            }
            else
            {
                V.push_back(L.tail->data);
                delete L.pop_back();
            }
        }
        std::cerr << "V1=" << V << std::endl;
        hsort(V, comparison::increasing<T> );
        std::cerr << "V2=" << V << std::endl;
    }

}
Y_UTEST(sort)
{
    perform<int>();
    perform<float>();
    perform<string>();
}
Y_UTEST_DONE()

