
#include "y/jive/pattern/all.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/string/convert.hpp"
#include "y/ios/icstream.hpp"

using namespace upsylon;
using namespace Jive;

namespace {


    static inline void test_pattern( const Pattern *p, Source &source )
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

        // check graphviz
        {
            const string dotname = basename + ".dot";
            motif->graphViz(dotname);
        }

        // test string/feeble
        std::cerr << "is feeble   : " << motif->feeble() << std::endl;
        std::cerr << "is strong   : " << motif->strong() << std::endl;

        // find first char
        FirstChars fc;
        motif->start(fc);
        FirstChars bc;
        bc.opposite(fc);
        std::cerr << "#first chars: " << fc.size() << std::endl;
        std::cerr << fc << std::endl;
        std::cerr << "#other chars: " << bc.size() << std::endl;
        std::cerr << bc << std::endl;
        Y_ASSERT(256==fc.size()+bc.size());
        std::cerr << "entropy     : " << motif->entropy() << std::endl;

        std::cerr << "\t<testing>" << std::endl;
        motif->test(source);
        std::cerr << "\t<testing/>" << std::endl;
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
    const char data[] = "abcdefghijklmnopqrstuvwxyz";
    Source     source( Module::OpenData("data",data) );

    test_pattern( Any :: Create(),source );
    test_pattern( newSingle()    ,source );
    test_pattern( newRange()     ,source );
    test_pattern( newExclude()   ,source );

    {
        arc_ptr<Logical> p = And::Create();
        p->push_back( newSingle() );
        p->push_back( newRange()  );
        test_pattern( & *p, source);
    }

    {
        arc_ptr<Logical> p = Or::Create();
        p->push_back( newSingle() );
        p->push_back( newRange()  );
        test_pattern( & *p, source);
    }


    {
        arc_ptr<Logical> p = None::Create();
        p->push_back( newSingle() );
        p->push_back( newRange()  );
        test_pattern( & *p, source);
    }

    test_pattern( Optional::Create(  newSingle() ), source );
    test_pattern( Repeating::Create( newSingle(),0 ), source );
    test_pattern( Repeating::Create( newSingle(),1 ), source );
    test_pattern( Repeating::Create( newSingle(),2 ), source );



    Y_UTEST_SIZEOF(Pattern);
    Y_UTEST_SIZEOF(Any);
    Y_UTEST_SIZEOF(Single);
    Y_UTEST_SIZEOF(Range);
    Y_UTEST_SIZEOF(Exclude);

    Y_UTEST_SIZEOF(Logical);
    Y_UTEST_SIZEOF(And);
    Y_UTEST_SIZEOF(Or);
    Y_UTEST_SIZEOF(None);

    Y_UTEST_SIZEOF(Optional);
    Y_UTEST_SIZEOF(Repeating);



}
Y_UTEST_DONE()



