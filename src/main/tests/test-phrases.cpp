#include "y/utest/run.hpp"
#include "y/string.hpp"
#include "y/counting/perm.hpp"
#include "y/counting/comb.hpp"
#include "y/ios/icstream.hpp"
#include "y/associative/suffix-tree.hpp"
#include "y/os/progress.hpp"
#include "y/core/node.hpp"
#include "y/ptr/arc.hpp"

using namespace upsylon;

namespace {



}

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
        for( perm.boot(); perm.good(); perm.next())
        {

            perm.apply(*motif,*alphabet);
            if(!dict.insert_by(motif,true))
            {
                continue;
            }

            if( bar.query() - old >= 0.5 )
            {
                bar.update(perm.index, perm.count);
                bar.display(std::cerr) << '\r';
                old = bar.query();
            }


        }
        bar.update(1.0);
        bar.display(std::cerr) << std::endl;
        std::cerr << "found [" << dict.entries() << "/" << perm.count << "]" << std::endl;




    }
}
Y_UTEST_DONE()


