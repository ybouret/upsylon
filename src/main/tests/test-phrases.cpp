#include "y/associative/b-tree.hpp"
#include "y/utest/run.hpp"
#include "y/string.hpp"
#include "y/counting/perm.hpp"
#include "y/counting/comb.hpp"
#include "y/ios/icstream.hpp"

using namespace upsylon;

Y_UTEST(phrases)
{
    if(argc>1)
    {
        const string  alphabet = argv[1];
        const size_t  letters  = alphabet.size();
        btree<string> dict;

        if( argc > 2 )
        {

            {
                ios::icstream fp( argv[2] );
                string        line;
                while( fp.gets(line) )
                {
                    (void)dict.insert_(line,line);
                }
            }
            std::cerr << "#dict=" << dict.entries() << std::endl;
        }

        const bool    hasDict = (dict.entries() > 0);
        if(hasDict) std::cerr << "use dictionary..." << std::endl;
        permutation   perm(letters);
        string        motif( letters, as_capacity, true);

        btree<string> motifs;

        for( perm.start(); perm.valid(); perm.next() )
        {
            perm.apply(*motif,*alphabet);
            if( !motifs.insert_( motif, motif ) ) continue;
            //std::cerr << "[" << motif << "]" << std::endl;
        }

        std::cerr << "[" << motifs.entries() << "/" << perm.count << "]" << std::endl;



    }
}
Y_UTEST_DONE()


