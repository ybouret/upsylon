#include "y/suffix/tree.hpp"
#include "y/utest/run.hpp"
#include "y/type/spec.hpp"
#include "y/ptr/auto.hpp"
#include "y/sequence/vector.hpp"
#include "y/sequence/list.hpp"

using namespace upsylon;
namespace
{
    template <typename CODE>
    void dispNode()
    {
        typedef suffix_node<CODE>  node_type;
        std::cerr << "-- suffix_node<" << type_name_of<CODE>() << "> \t: " << sizeof(node_type) << std::endl;
        auto_ptr<node_type>        root( new node_type(0,0,0) );
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
                node->leaves.push_back( new node_type(0,CODE(i),0) );
            }
        }
        typename node_type::pool_t pool;
        root->return_to(pool);
        root.dismiss();
    }

    template <typename CODE>
    void testTree()
    {
        const string &tid = type_name_of<CODE>();
        std::cerr << "-- suffix_tree<" << tid << ">" << std::endl;
        suffix_tree<CODE>    tree1;
        suffix_tree<CODE>    tree2;
        typedef vector<CODE> key_type;
        list<key_type> keys;
        void          *addr = suffix::in_use();
        std::cerr << "[";
        for(size_t iter=0;iter<32;++iter)
        {
            key_type key;
            for(size_t i=1+alea.leq(4);i>0;--i)
            {
                key << alea.range<CODE>('a','d');
            }
            if( tree1.grow( *key, key.size(), addr) )
            {
                keys << key;
                Y_ASSERT(tree2.grow(key,addr));
                Y_ASSERT(tree1==tree2);
                std::cerr << "+";
            }
            else
            {
                Y_ASSERT(!tree2.grow(key,addr));
                Y_ASSERT(tree1==tree2);
                std::cerr << "-";
            }
            Y_ASSERT(keys.size()==tree1.root->frequency);
        }
        std::cerr << "]" << std::endl;
        Y_ASSERT(keys.size()==tree1.root->frequency);
        Y_ASSERT(keys.size()==tree2.root->frequency);
        {
            const string fileName = "tree-" + tid + ".dot";
            tree1.root->graphViz(fileName);
        }

        tree2.erase();
        tree2.clone(tree1);
        Y_ASSERT(tree1==tree2);
        tree2.ditch();
        tree2.load_pool(1000);
        tree2.clone(tree1);
        Y_ASSERT(tree1==tree2);

    }
}


Y_UTEST(suffix)
{
    dispNode<uint8_t>();
    dispNode<uint16_t>();
    dispNode<uint32_t>();
    dispNode<uint64_t>();
    std::cerr << std::endl;

    testTree<uint8_t>();
    testTree<uint16_t>();
    testTree<uint32_t>();
    testTree<uint64_t>();


}
Y_UTEST_DONE()


