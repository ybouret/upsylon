#include "y/lang/pattern/posix.hpp"
#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"

using namespace upsylon;
using namespace Lang;

#define __CHECK(EXPR) do { auto_ptr<Pattern> p = Posix::EXPR(); p->GraphViz( #EXPR ".dot" ); } while(false)

Y_UTEST(posix)
{
    __CHECK(lower);
    __CHECK(upper);
    __CHECK(alpha);
    __CHECK(digit);
    __CHECK(alnum);
    __CHECK(space);
    __CHECK(blank);
    __CHECK(xdigit);
    __CHECK(word);
    __CHECK(endl);
    __CHECK(dot);
}
Y_UTEST_DONE()

