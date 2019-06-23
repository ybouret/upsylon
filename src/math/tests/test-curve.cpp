#include "y/math/geometric/curve.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include <typeinfo>
#include "y/ios/ocstream.hpp"

using namespace upsylon;
using namespace math;


#include "y/string/convert.hpp"
Y_UTEST(curve)
{
    size_t np = 3;
    if(argc>1)
    {
        np = string_convert::to<size_t>(argv[1],"np");
    }
    (void)np;
}
Y_UTEST_DONE()


