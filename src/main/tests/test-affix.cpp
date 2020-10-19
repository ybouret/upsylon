
#include "y/associative/affix/tree.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/ios/icstream.hpp"
#include "y/sequence/vector.hpp"
#include "y/string.hpp"
#include "y/hashing/sha1.hpp"

using namespace  upsylon;

static inline void build( core::affix &atree, const accessible<string> &strings)
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
        const string                 &data = strings[i];
        const core::affix::tree_node *node = atree.node_with(data);
        Y_ASSERT(node);
        Y_ASSERT(&data==node->addr);
    }

}

Y_UTEST(affix)
{
    Y_UTEST_SIZEOF(core::affix::tree_node);

    core::affix atree;

    if(argc>1)
    {
        hashing::sha1 H;
        vector<string> strings;
        {
            ios::icstream fp(argv[1]);
            string        line;
            while( fp.gets(line) )
            {
                strings << line;
            }
        }

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
    }

    if(atree.entries()<=20)
    {
        atree.graphViz("atree.dot");
    }

}
Y_UTEST_DONE()

