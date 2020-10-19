
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
    size_t inserted = 0;
    for(size_t i=1;i<=strings.size();++i)
    {
        const string &data = strings[i];
        const bool    res  = atree.insert_with(data, (void*)&data);
        if(res) ++inserted;
    }
    std::cerr << "inserted=" << inserted << std::endl;
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
        std::cerr << "md1=" << md1 << std::endl;

        alea.shuffle(*strings, strings.size() );
        build(atree,strings);
        atree.hash_with(H);
        const digest md2 = H.md();
        std::cerr << "md2=" << md2 << std::endl;
    }

    atree.graphViz("atree.dot");


}
Y_UTEST_DONE()

