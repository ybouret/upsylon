
#include "y/jive/pattern/first-chars.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/utest/sizeof.hpp"

using namespace upsylon;
using namespace Jive;

Y_UTEST(jive_fc)
{

    FirstChars fc;
    fc.fulfill();
    //std::cerr << "fc=" << fc << std::endl;
}
Y_UTEST_DONE()
