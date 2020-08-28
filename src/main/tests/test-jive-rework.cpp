#include "y/jive/pattern/basic/rework.hpp"
#include "y/jive/pattern/logic/all.hpp"

#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"

using namespace upsylon;
using namespace Jive;

Y_UTEST(jive_rework)
{

    {
        auto_ptr<Logical> p = Or::Create();
        auto_ptr<Logical> q = None::Create();

    }

}
Y_UTEST_DONE()

