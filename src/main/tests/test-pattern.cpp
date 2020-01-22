
#include "y/lang/pattern/basic.hpp"
#include "y/lang/pattern/logic.hpp"
#include "y/lang/pattern/joker.hpp"
#include "y/ios/imstream.hpp"

#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Lang;

namespace
{
    static inline void test_io( const Pattern &p )
    {
        const string bin = p.toBinary();

        auto_ptr<Pattern> q = 0;
        {
            ios::imstream fp(bin);
            q = Pattern::Load(fp);
            Y_ASSERT( q->toBinary() == bin );
        }
    }
}

Y_UTEST(pattern)
{
    auto_ptr<Pattern> p = new Any1();
    p->GraphViz("any1.dot");
    test_io(*p);

    p = new Single('\\');
    p->GraphViz("single.dot");
    test_io(*p);

    p = new Range('a','z');
    p->GraphViz("range.dot");
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
    q->GraphViz("and.dot");
    test_io(*q);

    q = new AND();
    q->add( Optional ::Create( new Single('A') )    );
    q->add( Repeating::Create( new Single('B'), 0 ) );
    q->add( Repeating::Create( new Single('C'), 1 ) );
    q->add( Repeating::Create( new Single('D'), 2 ) );
    q->add( Counting ::Create( new Single('E'),0,5) );
    q->GraphViz("jk.dot");
    test_io(*q);

}
Y_UTEST_DONE()
