#include "y/utest/run.hpp"
#include "y/string.hpp"
#include "y/counting/part.hpp"
#include "y/counting/permuter.hpp"
#include "y/ios/icstream.hpp"
#include "y/associative/suffix-table.hpp"

using namespace upsylon;

namespace {



}

Y_UTEST(phrases)
{
    if(argc>1)
    {
        const string                alphabet = argv[1];
        const size_t                letters  = alphabet.size();
        permuter<char>              perm( *alphabet, alphabet.size() );
        integer_partition           part(letters);

        std::cerr << "#permutation : " << perm.count      << std::endl;
        std::cerr << "#outcome     : " << part.outcomes() << std::endl;
        suffix_table<string,string> dict;
        if(argc>2)
        {
            ios::icstream fp( argv[2] );
            string        line;
            while(fp.gets(line))
            {
                (void) dict.insert(line,line);
            }
            std::cerr << "#dict=" << dict.size() << std::endl;
        }
        
        do
        {
            std::cerr << "<" << part << ">" << std::endl;
            permuter<size_t> block( part );
            std::cerr << "_#blocks=" << block.count << std::endl;
            
            for( block.boot(); block.good(); block.next() )
            {
                std::cerr << "\t-> " << block << std::endl;
            }
            
        } while(part.build_next());
        
        
        
        

    }
}
Y_UTEST_DONE()


