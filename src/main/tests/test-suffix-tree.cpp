#include "y/associative/suffix-table.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/string.hpp"
#include "y/ios/icstream.hpp"
#include "y/sequence/vector.hpp"
#include "y/comparison.hpp"
#include "y/hashing/sha1.hpp"

using namespace upsylon;

namespace {
    static inline int compare_strings_by_length( const string &lhs, const string &rhs ) throw()
    {
        const size_t L = lhs.size();
        const size_t R = rhs.size();
        return comparison::increasing(L,R);
    }

    template <typename ITERATOR>
    void display(ITERATOR curr, const ITERATOR last)
    {
        std::cerr << '[';
        while( curr != last )
        {
            std::cerr << *(curr++) << '/';
        }
        std::cerr << '#' << ']' << std::endl;
    }

    static inline bool doCallback(const suffix_path &path,
                                  const string      &data)
    {
        
        return path.size() == data.size();
    }

}



Y_UTEST(stree)
{

    hashing::sha1 H;

    suffix_tree<int>    itree;
    suffix_tree<string> stree;
    vector<string>      keys;

    suffix_table<string,long>  ltable;
    suffix_table<uint64_t,int> utable;

    std::cerr << "inserting keys" << std::endl;
    size_t collisions = 0;
    if(argc>1)
    {
        ios::icstream fp( argv[1] );
        string        line;
        int           i=0;
        while( fp.gets(line) )
        {
            keys << line;
            ++i;
            if(itree.insert_by(line,i))
            {
                Y_ASSERT(stree.insert_by(line,line));
                Y_ASSERT(ltable.insert(line,i));
            }
            const uint64_t h = H.key<uint64_t>(line);
            if(!utable.insert(h,i))
            {
                ++collisions;
            }
        }
    }
    std::cerr << "#itree      : " << itree.entries() << std::endl;
    std::cerr << "#stree      : " << stree.entries() << std::endl;
    std::cerr << "#collisions : " << collisions      << std::endl;

    const bool shallSave =itree.entries() <= 100;
    if(  shallSave )
    {
        itree.get_root().graphViz("itree.dot");
        display( itree.begin(),  itree.end()  );
        const suffix_tree<int> &cu = utable;
        display( cu.begin(), cu.end() );
    }

    itree.sort_with(  comparison::increasing<int> );
    stree.sort_with(  compare_strings_by_length   );
    utable.sort_with( comparison::decreasing<int> );

    Y_ASSERT(stree.for_each(doCallback));
    
    {
        const suffix_tree<int>    itree2(itree);
        Y_CHECK(itree2.entries()==itree.entries());

    }

    {
        const suffix_tree<string> stree2(stree);
        Y_CHECK(stree2.entries()==stree.entries());
        if( stree2.entries() <= 100 )
        {
            stree2.get_root().graphViz("stree.dot");
        }
    }

    std::cerr << "look for keys" << std::endl;
    alea.shuffle( *keys, keys.size() );
    for(size_t i=1;i<=keys.size();++i)
    {
        Y_ASSERT( itree.look_for( *keys[i], keys[i].size() ) );
        Y_ASSERT( stree.has( keys[i] ) );
    }

    std::cerr << "removing some keys" << std::endl;
    alea.shuffle( *keys, keys.size() );
    vector<string> removed;
    for(size_t i=keys.size()/2;i>0;--i)
    {
        removed.push_back( keys.back() );
        keys.pop_back();
    }

    for(size_t i=1;i<=removed.size();++i)
    {
        Y_ASSERT( itree.remove_with( *removed[i], removed[i].size() ) );
        Y_ASSERT( stree.remove_by( removed[i] ) );
    }

    std::cerr << "testing keys on/off"  << std::endl;
    for(size_t i=1;i<=removed.size();++i)
    {
        Y_ASSERT( !itree.has( removed[i] ) );
        Y_ASSERT( !stree.has( removed[i] ) );
    }

    for(size_t i=keys.size();i>0;--i)
    {
        Y_ASSERT( itree.has( keys[i] ) );
        Y_ASSERT( stree.has( keys[i] ) );
    }

    if( shallSave )
    {
        itree.get_root().graphViz("itree_clr.dot");
        itree.trim();
        itree.get_root().graphViz("itree_trm.dot");

    }
    
    itree.release_all();
    


    std::cerr << "end..." << std::endl;

    Y_UTEST_SIZEOF( suffix_tree<int>::data_node );
    Y_UTEST_SIZEOF( suffix_tree<int>::node_type );

    Y_UTEST_SIZEOF( suffix_tree<string>::data_node );
    Y_UTEST_SIZEOF( suffix_tree<string>::node_type );

    Y_UTEST_SIZEOF( suffix_tree<uint8_t>::data_node );
    Y_UTEST_SIZEOF( suffix_tree<uint8_t>::node_type );
}
Y_UTEST_DONE()
