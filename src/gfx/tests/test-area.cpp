
#include "y/gfx/area.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace gfx;

namespace
{
    static unit_t pos()
    {
        return alea.range<unit_t>(-100,100);
    }
}

Y_UTEST(area)
{
    area z(0,0, pos(), pos());    std::cerr << "z    = " << z   << std::endl;
    area one(1,1,pos(),pos());  std::cerr << "one  = " << one << std::endl;
    
    area a( 1+alea.leq(100), 1+alea.leq(100), pos(), pos() ); std::cerr << "a    = " << a << std::endl;
    
}
Y_UTEST_DONE()

