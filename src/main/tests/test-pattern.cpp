
#include "y/lang/pattern/basic.hpp"
#include "y/lang/pattern/logic.hpp"
#include "y/lang/pattern/joker.hpp"
#include "y/ios/imstream.hpp"

#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"

using namespace upsylon;
using namespace Lang;

namespace
{
    static inline void test_io( const Pattern &p, const char *saveName=NULL)
    {
        std::cerr << "Testing <" << p.className() << ">" << std::endl;
        
        const string bin = p.to_binary();
        if(saveName)
        {
            string fileName = saveName;
            fileName += ".bin";
            ios::ocstream fp(fileName);
            fp << bin;
        }
        auto_ptr<Pattern> q = 0;
        {
            ios::imstream fp(bin);
            q = Pattern::Load(fp);
            Y_ASSERT( q->to_binary() == bin );
        }
    }
}

Y_UTEST(pattern)
{

    Y_UTEST_SIZEOF(Pattern);

    Y_UTEST_SIZEOF(Any1);
    Y_UTEST_SIZEOF(Single);
    Y_UTEST_SIZEOF(Range);

    Y_UTEST_SIZEOF(AND);
    Y_UTEST_SIZEOF(OR);
    Y_UTEST_SIZEOF(NONE);

    Y_UTEST_SIZEOF(Optional);
    Y_UTEST_SIZEOF(Repeating);
    Y_UTEST_SIZEOF(Counting);

    auto_ptr<Pattern> p = new Any1();
    p->graphViz("any1.dot");
    test_io(*p);

    p = new Single('\\');
    p->graphViz("single.dot");
    test_io(*p);

    p = new Range('a','z');
    p->graphViz("range.dot");
    test_io(*p);

    auto_ptr<Logical> q = new AND();
    q->add(new Single('a'));
    q->add(new Single('b'));
    q->add(new Any1());
    {
        Logical *sub = q->add( new OR() );
        sub->add(new Single('c'));
        sub->add(new Range('A','Z'));
    }
    q->add(new Single('p'));
    q->graphViz("and.dot");
    test_io(*q);

    q = new AND();
    q->add( Optional ::Create( new Single('A') )    );
    q->add( Repeating::Create( new Single('B'), 0 ) );
    q->add( Repeating::Create( new Single('C'), 1 ) );
    q->add( Repeating::Create( new Single('D'), 2 ) );
    q->add( Counting ::Create( new Single('E'),0,5) );
    q->graphViz("jk.dot");

    test_io(*q,"AND");

    q = new OR();
    test_io(*q,"OR");


    q = new NONE();
    test_io(*q,"NONE");

    p =  Optional ::Create( new Single('A') );
    test_io(*p,"OPT");

    p =  Repeating::Create( new Single('A'), 1);
    test_io(*p,"REP");

    p =  Counting::Create( new Single('A'), 1,2);
    test_io(*p,"CNT");

}
Y_UTEST_DONE()
