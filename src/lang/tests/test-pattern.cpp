
#include "y/lang/pattern/basic.hpp"
#include "y/lang/pattern/logic.hpp"

#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Lang;

Y_UTEST(pattern)
{
    auto_ptr<Pattern> p = new Any1();
    p->GraphViz("any1.dot");
    p = new Single('\\');
    p->GraphViz("single.dot");

    p = new Range('a','z');
    p->GraphViz("range.dot");

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
    

}
Y_UTEST_DONE()
