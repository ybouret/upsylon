
#include "y/associative/affix/tree.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/ios/icstream.hpp"
#include "y/sequence/vector.hpp"
#include "y/string.hpp"
#include "y/hashing/sha1.hpp"
#include "y/associative/be-key.hpp"

using namespace  upsylon;

static inline void build(affix &atree, const accessible<string> &strings)
{
    atree.clear();
    std::cerr << "in_pool: " << std::setw(6) << atree.in_pool() << " ";
    atree.gc(1024);
    size_t inserted = 0;
    for(size_t i=1;i<=strings.size();++i)
    {
        const string &data = strings[i];
        const bool    res  = atree.insert_with(data, (void*)&data);
        if(res) ++inserted;
    }
    std::cerr << "inserted=" << inserted;
    Y_ASSERT(atree.entries()==inserted);
    for(size_t i=strings.size();i>0;--i)
    {
        const string           &data = strings[i];
        const affix::tree_node *node = atree.node_with(data);
        Y_ASSERT(node);
        Y_ASSERT(&data==node->addr);
    }

}

static inline void low_level( vector<string> &strings )
{
    affix          atree;
    hashing::sha1  H;


    alea.shuffle(*strings, strings.size() );
    build(atree,strings);
    atree.hash_with(H);
    const digest md1 = H.md();
    std::cerr << " md1=" << md1 << std::endl;

    for(size_t iter=0;iter<8;++iter)
    {
        alea.shuffle(*strings, strings.size() );
        build(atree,strings);
        atree.hash_with(H);
        const digest md2 = H.md();
        std::cerr << " md2=" << md2 << std::endl;
        Y_ASSERT(md1==md2);
    }

    const bool save = atree.entries() <= 30;
    if(save) atree.graphViz("atree.dot");


    Y_ASSERT(strings.size()==atree.entries());
    vector<size_t> indx(strings.size(),0);
    for(size_t i=strings.size();i>0;--i) indx[i] = i;

    for(size_t iter=0;iter<8;++iter)
    {
        alea.shuffle(*strings, strings.size() );
        build(atree,strings);

        alea.shuffle(*indx,indx.size());
        const size_t half = indx.size()/2;
        for(size_t i=half;i>0;--i)
        {
            const string           &data = strings[indx[i]];
            const affix::tree_node *node = atree.node_with(data);
            Y_ASSERT(node);
            Y_ASSERT(node->addr);
            Y_ASSERT(node->addr == &data );
            atree.remove_node( (affix::tree_node *)node );
        }


        for(size_t i=indx.size();i>half;--i)
        {
            const string           &data = strings[indx[i]];
            const affix::tree_node *node = atree.node_with(data);
            Y_ASSERT(node);
            Y_ASSERT(node->addr);
            Y_ASSERT(node->addr == &data );
            atree.remove_node( (affix::tree_node *)node );
        }

        std::cerr << std::endl;
    }

    // using addresses
    atree.clear();
    for(size_t i=strings.size();i>0;--i)
    {
        const string &data = strings[i];
        const be_key key( data );
        Y_ASSERT(atree.insert_with(key,(void*)&data));
    }
    if(atree.entries()<=30) atree.graphViz("addresses.dot");
}

static inline void high_level( vector<string> &strings )
{
    affix_tree<string> tree;

    for(size_t i=strings.size();i>0;--i)
    {
        const string &data = strings[i];
        tree.insert_at(*data, data.length(), data);
        break;
    }

}


Y_UTEST(affix)
{
    Y_UTEST_SIZEOF(affix::tree_node);


    if(argc>1)
    {
        vector<string> strings;
        {
            ios::icstream fp(argv[1]);
            string        line;
            while( fp.gets(line) )
            {
                strings << line;
            }
        }
        low_level(strings);
        high_level(strings);

    }







}
Y_UTEST_DONE()

