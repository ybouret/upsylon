
#include "y/jive/pattern/all.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/string/convert.hpp"
#include "y/ios/icstream.hpp"

using namespace upsylon;
using namespace Jive;

namespace {


    static inline void test_pattern( const Pattern *p )
    {
        const Motif  motif    = p;
        const string cls_name = motif->className();
        std::cerr << "<" << cls_name << ">" << std::endl;
        Y_CHECK(motif->self != NULL);
        string       basename = cls_name;string_convert::to_lower(basename);
        const string bin      = motif->to_binary();

        // check cloning
        {
            const Motif  motif_copy = motif->clone();
            Y_CHECK(motif_copy->to_binary() == bin);
            Y_CHECK(motif_copy->self        != NULL);
        }

        // check IO
        {
            const string savename = basename+ ".bin";
            motif->save_to(savename);
            {
                ios::icstream fp(savename);
                const Motif reloaded = Pattern::Load(fp);
                Y_CHECK(reloaded->to_binary()==bin);
            }
        }

        {
            const string dotname = basename + ".dot";
            motif->graphViz(dotname);
        }

        FirstChars fc;
        motif->start(fc);
        std::cerr << "is feeble   : " << motif->feeble() << std::endl;
        std::cerr << "is strong   : " << motif->strong() << std::endl;
        std::cerr << "#first chars: " << fc.size()       << std::endl;
        std::cerr << fc << std::endl;
        std::cerr << "<" << cls_name << "/>" << std::endl << std::endl;

    }

    static inline Pattern * newSingle()
    {
        return Single :: Create(alea.range('a','z'));
    }

    static inline Pattern * newExclude()
    {
        return Exclude :: Create(alea.range('a','z'));
    }

    static inline Pattern * newRange()
    {
        return  Range  :: Create(alea.range('a','z'),alea.range('a','z') );
    }
}

Y_UTEST(jive_patterns)
{
    test_pattern( Any :: Create()    );
    test_pattern( newSingle() );
    test_pattern( newRange()  );
    test_pattern( newExclude() );

    {
        arc_ptr<And> p = And::Create();
        p->add( newSingle() );
        p->add( newRange()  );
        test_pattern( & *p );
    }

    Y_UTEST_SIZEOF(Pattern);
    Y_UTEST_SIZEOF(Any);
    Y_UTEST_SIZEOF(Single);
    Y_UTEST_SIZEOF(Range);
    Y_UTEST_SIZEOF(Exclude);
    Y_UTEST_SIZEOF(Logical);


}
Y_UTEST_DONE()



