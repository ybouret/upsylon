
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
    

}
Y_UTEST_DONE()
