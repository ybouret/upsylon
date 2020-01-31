
#include "y/lang/pattern/common.hpp"
#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"

using namespace upsylon;
using namespace Lang;


#define BUILD(TT) do { auto_ptr<Pattern> p = RegExp(Common::TT); \
p->graphViz( "Common_" #TT ".dot" );\
} while(false)

Y_UTEST(common)
{
    BUILD(INTEGER);
    BUILD(POSITIVE_DOUBLE);
    BUILD(C_IDENTIFIER);
    BUILD(DOUBLE);
}
Y_UTEST_DONE()


