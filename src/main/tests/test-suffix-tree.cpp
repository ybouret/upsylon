#include "y/associative/suffix-tree.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/string.hpp"
#include "y/ios/icstream.hpp"
#include "y/sequence/vector.hpp"

using namespace upsylon;

Y_UTEST(stree)
{

    Y_UTEST_SIZEOF( suffix_tree<int>::data_node );
    Y_UTEST_SIZEOF( suffix_tree<int>::tree_node );

    Y_UTEST_SIZEOF( suffix_tree<string>::data_node );
    Y_UTEST_SIZEOF( suffix_tree<string>::tree_node );


    suffix_tree<int>    itree;
    suffix_tree<string> stree;
    vector<string>      keys;

    std::cerr << "inserting keys" << std::endl;
    if(argc>1)
    {
        ios::icstream fp( argv[1] );
        string        line;
        int           i=0;
        while( fp.gets(line) )
        {
            keys << line;
            const char *k = *line;
            ++i;
            itree.insert_with(k, line.size(), i);
            stree.insert_with(k, line.size(), line);
        }
    }

    std::cerr << "look for keys" << std::endl;
    alea.shuffle( *keys, keys.size() );
    for(size_t i=1;i<=keys.size();++i)
    {
        Y_ASSERT( itree.look_for( *keys[i], keys[i].size() ) );
    }

    std::cerr << "removing some keys" << std::endl;
    alea.shuffle( *keys, keys.size() );
    for(size_t i=keys.size()/2;i>0;--i)
    {
        Y_ASSERT( itree.remove_with( *keys[i], keys[i].size() ) );
    }

    if( itree.entries() <= 100 )
    {
        itree.get_root().graphViz("itree.dot");
    }

    std::cerr << "end..." << std::endl;

}
Y_UTEST_DONE()
