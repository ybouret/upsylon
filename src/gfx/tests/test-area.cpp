
#include "y/gfx/area.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace graphic;

namespace
{
    static unit_t pos()
    {
        return alea.range<unit_t>(-100,100);
    }
}

Y_UTEST(area)
{
    area z(0,0, pos(), pos());  std::cerr << "z    = " << z   << std::endl;
    area one(1,1,pos(),pos());  std::cerr << "one  = " << one << std::endl;

    for(size_t iter=0;iter<16;++iter)
    {
        area a( 1+alea.leq(100), 1+alea.leq(100), pos(), pos() ); std::cerr << "a    = " << a << std::endl;
        for(unit_t y=a.lower.y;y<=a.upper.y;++y)
        {
            for(unit_t x=a.lower.x;x<=a.upper.x;++x)
            {
                const unit_t i = a.index_of(x,y);
                Y_ASSERT(i>=0);
                Y_ASSERT(i<a.items);
                const coord  p = a.coord_of(i);
                Y_ASSERT(x==p.x);
                Y_ASSERT(y==p.y);
            }
        }
    }
}
Y_UTEST_DONE()

