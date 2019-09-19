#include "y/associative/b-table.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include "y/sequence/vector.hpp"
#include "y/memory/cblock.hpp"
#include "y/ptr/shared.hpp"
#include "y/ios/ocstream.hpp"
#include "y/ios/icstream.hpp"

#include <typeinfo>
#include "y/ios/tools/graphviz.hpp"

using namespace upsylon;

namespace
{
    typedef memory::cblock_of<uint8_t> key_type;
    typedef shared_ptr<key_type>       shared_key;

    template <typename T>
    static inline void do_btree(bool save)
    {
        const string id = typeid(T).name();
        std::cerr << "btree<" << id << ">" << std::endl;
        std::cerr << "sizeof(node_type)=" << sizeof(typename btree<T>::node_type) << std::endl;
        btree<T>           tree;
        vector<shared_key> keys;

        std::cerr << "building..." << std::endl;
        for(size_t i=40+alea.leq(40);i>0;--i)
        {
            const T    tmp = support::get<T>();
            shared_key key = new key_type( alea.leq(3) );
            for(size_t j=0;j<key->size;++j)
            {
                key->data[j] = uint8_t( alea.leq(4) );
            }

            if( tree.insert_(key->data, key->size, tmp) )
            {
                keys.push_back(key);
            }
        }

        Y_CHECK( tree.entries() == keys.size() );
        std::cerr << "\t#tree.entries=" << tree.entries() << std::endl;

        if(save)
        {
            const string fn = "btree_" + id + ".dot";
            {
                ios::ocstream fp(fn);
                tree.graphviz(fp);
            }
            ios::GraphViz::Render(fn);
        }

        std::cerr << "searching..." << std::endl;
        alea.shuffle(*keys, keys.size() );
        for(size_t i=keys.size();i>0;--i)
        {
            const key_type &k = *keys[i];
            Y_ASSERT( tree.search_( k.data, k.size ) );
        }

        std::cerr << std::endl;


    }


    template <typename ITERATOR>
    void display_range( ITERATOR it, const ITERATOR end)
    {
        std::cerr << '{';
        for(;it!=end;++it)
        {
            std::cerr << ' ' << *it;
        }
        std::cerr << '}' << std::endl;
    }


}

Y_UTEST(btree)
{

    do_btree<unsigned short>(true);
    do_btree<string>(false);

    
    if( argc > 1 )
    {
        btree<unsigned>         tree;
        btable<string,unsigned> table;
        
        vector<string>  keys;
        {
            const string    fn = argv[1];
            ios::icstream   fp(fn);
            string          line;
            unsigned        count1=0;
            unsigned        count2=0;
            while( fp.gets(line) )
            {
                if(tree.insert_(line,count1))
                {
                    ++count1;
                    keys.push_back(line);
                }

                if(table.insert(line,count1))
                {
                    ++count2;
                }

                Y_ASSERT(count1==count2);
            }
        }
        std::cerr << "\t#tree.entries=" << tree.entries() << std::endl;

        alea.shuffle(*keys,keys.size());

        for(size_t i=keys.size();i>0;--i)
        {
            Y_ASSERT( tree.search_(keys[i]) );
        }
        display_range(table.begin(),table.end());
        display_range(table.rbegin(),table.rend());
        {
            const btable<string,unsigned> &cst = table;
            display_range(cst.begin(),  cst.end());
            display_range(cst.rbegin(), cst.rend());
        }

        if(keys.size()<=100)
        {
            const string fn = "btree.dot";
            {
                ios::ocstream fp(fn);
                tree.graphviz(fp);
            }
            ios::GraphViz::Render(fn);
        }
    }



}
Y_UTEST_DONE()





