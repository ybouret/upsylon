

#include "y/gfx/bitmap.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"

using namespace upsylon;
using namespace graphic;

Y_UTEST(bitmap)
{
    Y_UTEST_SIZEOF(area);
    Y_UTEST_SIZEOF(bitrow);
    Y_UTEST_SIZEOF(bitrows);
    

}
Y_UTEST_DONE()

