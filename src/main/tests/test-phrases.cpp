#include "y/utest/run.hpp"
#include "y/string.hpp"
#include "y/counting/perm.hpp"
#include "y/counting/comb.hpp"
#include "y/ios/icstream.hpp"
#include "y/associative/suffix-tree.hpp"
#include "y/os/progress.hpp"

using namespace upsylon;

Y_UTEST(phrases)
{
    if(argc>1)
    {
        const string      alphabet = argv[1];
        const size_t      letters  = alphabet.size(); std::cerr << "'" << alphabet << "'" << std::endl; std::cerr << "#card=" << letters << std::endl;
        permutation       perm(letters);              std::cerr << "#perm=" << perm.count << std::endl;
        suffix_tree<bool> dict(perm.count,as_capacity);
        string            motif(letters,as_capacity,true);

        progress          bar;
        bar.start();
        double old = bar.query();
        for( perm.start(); perm.valid(); perm.next())
        {
            if( bar.query() - old >= 0.5 )
            {
                bar.update(perm.index, perm.count);
                bar.display(std::cerr) << '\r';
                old = bar.query();
            }
            perm.apply(*motif,*alphabet);
            if(!dict.insert_by(motif,true))
            {
                continue;
            }

            


        }
        bar.update(1.0);
        bar.display(std::cerr) << std::endl;
        std::cerr << "found [" << dict.entries() << "/" << perm.count << "]" << std::endl;


#if 0
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
#endif


    }
}
Y_UTEST_DONE()


