#include "y/associative/btree.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include "y/sequence/vector.hpp"
#include "y/memory/cblock.hpp"
#include "y/ptr/shared.hpp"
#include "y/ios/ocstream.hpp"
#include <typeinfo>
#include "y/ios/tools/graphviz.hpp"

using namespace upsylon;

namespace
{
    typedef memory::cblock_of<uint8_t> key_type;
    typedef shared_ptr<key_type>       shared_key;

    template <typename T>
    static inline void do_btree()
    {
        const string id = typeid(T).name();
        std::cerr << "btree<" << id << ">" << std::endl;
        std::cerr << "sizeof(node_type)=" << sizeof(typename btree<T>::node_type) << std::endl;
        btree<T>           tree;
        vector<shared_key> keys;

        for(size_t i=40+alea.leq(40);i>0;--i)
        {
            const T    tmp = support::get<T>();
            shared_key key = new key_type( alea.leq(3) );
            for(size_t j=0;j<key->size;++j)
            {
                key->data[j] = uint8_t( alea.leq(4) );
            }

            if( tree.insert(key->data, key->size, tmp) )
            {
                //std::cerr << "inserted [" << tmp << "]" << std::endl;
                keys.push_back(key);
            }
        }

        Y_CHECK( tree.size() == keys.size() );
        std::cerr << "#tree.size=" << tree.size() << "/nodes=" << tree.internal() << std::endl;

        {
            const string fn = "btree_" + id + ".dot";
            {
                ios::ocstream fp(fn);
                tree.graphviz(fp);
            }
            ios::GraphViz::Render(fn);
        }

        alea.shuffle(*keys, keys.size() );
        std::cerr << std::endl;


    }
}

Y_UTEST(btree)
{
    

    do_btree<unsigned short>();
    //do_btree<string>();


}
Y_UTEST_DONE()





