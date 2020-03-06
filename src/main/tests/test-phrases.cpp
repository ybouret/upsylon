#include "y/utest/run.hpp"
#include "y/string.hpp"
#include "y/counting/perm.hpp"
#include "y/counting/comb.hpp"
#include "y/ios/icstream.hpp"
#include "y/ordered/sorted-vector.hpp"
#include "y/associative/map.hpp"

using namespace upsylon;

typedef sorted_vector<string>       svec_type;
typedef ordered_single< svec_type > dict_type;
//typedef ordered_unique< svec_type > db_type;
typedef map<string,string> db_type;

Y_UTEST(phrases)
{
    if(argc>1)
    {
        const string  alphabet = argv[1];
        const size_t  letters  = alphabet.size();
        dict_type     dict( 32 * 1024, as_capacity );

        if( argc > 2 )
        {

            {
                ios::icstream fp( argv[2] );
                string        line;
                while( fp.gets(line) )
                {
                    dict.insert(line);
                }
            }
            std::cerr << "#dict=" << dict.size() << std::endl;
        }

        const bool    hasDict = (dict.size() > 0);
        if(hasDict) std::cerr << "use dictionary..." << std::endl;
        permutation   perm(letters);
        string        motif( letters, as_capacity, true);

        db_type motifs( perm.count, as_capacity );
        std::cerr << "studying #perm=" << perm.count << " [";
        for( perm.start(); perm.valid(); perm.next() )
        {
            perm.apply(*motif,*alphabet);
            if( !motifs.insert(motif,motif) ) continue;
            const size_t n = motifs.size();
            if( 0 == (n%1000) )
            {
                ( std::cerr << '.' ).flush();
            }
            //std::cerr << "[" << motif << "]" << std::endl;
        }

        std::cerr << "]\n[" << motifs.size() << "/" << perm.count << "]" << std::endl;



    }
}
Y_UTEST_DONE()


