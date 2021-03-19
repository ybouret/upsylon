#include "y/gfx/ops/blur.hpp"
#include "y/utest/run.hpp"
#include "y/gfx/image/io.hpp"

using namespace upsylon;
using namespace graphic;

namespace
{


}

Y_UTEST(blur)
{
    Y_USE_IMG();

    for(float sigma=0.1f;sigma<=6.0f;sigma += 0.1f)
    {
        blur b(sigma);
    }

    {
        blur b(10);
        IMG.save(b.weight, "blur.png");
    }

}
Y_UTEST_DONE()

