#include "y/suffix/node.hpp"
#include "y/utest/run.hpp"
#include "y/type/spec.hpp"
#include "y/ptr/auto.hpp"

using namespace upsylon;
namespace
{
    template <typename CODE>
    void dispNode()
    {
        typedef suffix_node<CODE>  node_type;
        std::cerr << "-- suffix_node<" << type_name_of<CODE>() << "> \t: " << sizeof(node_type) << std::endl;
        auto_ptr<node_type>        root( new node_type(0,0,0) );
        typename node_type::pool_t pool;
        for(size_t i=1+alea.leq(100);i>0;--i)
        {
            root->leaves.push_back( new node_type(0,CODE(i),0) );
            node_type *node = root->leaves.tail;
            if(alea.choice())
            {
               node->addr = suffix::in_use();
            }

            for(size_t j=1+alea.leq(10);j>0;--j)
            {
                node->new_free_child(pool,CODE(j));
            }
        }
        alea.shuffle(pool);
    }
}
Y_UTEST(suffix)
{
    dispNode<uint8_t>();
    dispNode<uint16_t>();
    dispNode<uint32_t>();
    dispNode<uint64_t>();

}
Y_UTEST_DONE()


