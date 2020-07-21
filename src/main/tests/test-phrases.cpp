#include "y/utest/run.hpp"
#include "y/string.hpp"
#include "y/counting/part.hpp"
#include "y/counting/permutations.hpp"
#include "y/ios/icstream.hpp"
#include "y/associative/suffix/table.hpp"
#include "y/sequence/vector.hpp"

using namespace upsylon;

namespace {



}

Y_UTEST(phrases)
{
    if(argc>1)
    {
        const string                alphabet = argv[1];
        const size_t                letters  = alphabet.size();
        permutations_of<char>       perm( *alphabet, alphabet.size() );
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
        
        size_t         total = 0;
        vector<string> phrase(letters,as_capacity);
        do
        {
            //! take an integer partition
            //std::cerr << "-- with: <" << part << "> => ";
            
            //! prepare all possible arrangements
            permutations_of<size_t> arrangement( part );
            //std::cerr << " #arrangement=" << arrangement.count << std::endl;
            
            for( arrangement.boot(); arrangement.good(); arrangement.next() )
            {
                //std::cerr << "--       <" << arrangement << ">" << std::endl;
                const size_t blocks = arrangement.size();
                if(blocks==letters) break;
                size_t num1 = 0;
                for(size_t i=blocks;i>0;--i)
                {
                    if( 1 == arrangement[i] )
                    {
                        ++num1;
                    }
                }
                if(num1>1)
                {
                    break;
                }
                for(perm.boot();perm.good();perm.next())
                {
                    phrase.free();
                    size_t i=1;
                    for(size_t block=1;block<=blocks;++block)
                    {
                        size_t rep = arrangement[block];
                        string word(rep,as_capacity,false);
                        while(rep-- > 0)
                        {
                            word << perm[i++];
                        }
                        phrase << word;
                    }
                    if(dict.size()>0)
                    {
                        bool ok = true;
                        for(size_t i=phrase.size();i>0;--i)
                        {
                            if( !dict.search(phrase[i]))
                            {
                                ok = false;
                                break;
                            }
                        }
                        if(ok)
                        {
                            std::cerr << phrase << std::endl;
                        }
                    }
                    else
                    {
                        std::cerr << phrase << std::endl;
                    }
                    ++total;
                }
            }
            //std::cerr << "-- done" << std::endl << std::endl;

        } while(part.build_next());
        std::cerr << "#phrases=" << total << std::endl;

    }
}
Y_UTEST_DONE()


