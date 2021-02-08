#include "y/gfx/types.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace GFX;

Y_UTEST(types)
{
    std::cerr << Check::Width  << std::endl;
    std::cerr << Check::Height << std::endl;

    for(unsigned i=0;i<9;++i)
    {
        std::cerr << i << " => " << Position::Text(i) << std::endl;
    }

}
Y_UTEST_DONE()

