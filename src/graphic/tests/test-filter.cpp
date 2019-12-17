#include "y/graphic/ops/filter.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"

using namespace upsylon;
using namespace Graphic;

namespace  {

    template <typename T>
    static inline void fillField( Oxide::Field2D<T> &F )
    {
        for(unit_t y=F.lower.y;y<=F.upper.y;++y)
        {
            for(unit_t x=F.lower.x;x<=F.upper.x;++x)
            {
                F[y][x] = support::get<T>();
            }
        }

    }
}

Y_UTEST(filter)
{
    Filter<float> F("f1",Point(-1,-1),Point(1,1));
    fillField(F);
    F.compile();
}
Y_UTEST_DONE()

