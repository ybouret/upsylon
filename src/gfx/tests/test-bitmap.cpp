

#include "y/gfx/bitmap.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"

using namespace upsylon;
using namespace gfx;

Y_UTEST(bitmap)
{
    Y_UTEST_SIZEOF(gfx::area);
    Y_UTEST_SIZEOF(gfx::bitrow);

}
Y_UTEST_DONE()

