#include "y/gfx/area/patch.hpp"
#include "y/utest/run.hpp"
#include "y/type/spec.hpp"
#include "support.hpp"
#include "y/gfx/color/rgb.hpp"

using namespace upsylon;
using namespace graphic;

namespace {

    template <typename T>
    inline void do_patch()
    {
        std::cerr << "patch<" << type_name_of<T>() << ">" << std::endl;


        patch<T> p(alea.range<unit_t>(1,1000), alea.range<unit_t>(1,1000),alea.range<unit_t>(-100,100),alea.range<unit_t>(-100,100));

        std::cerr << "area:" << p << " => " << p.allocated() << " bytes" << std::endl;

        for(unit_t y=p.lower.y;y<=p.upper.y;++y)
        {
            for(unit_t x=p.lower.x;x<=p.upper.x;++x)
            {
                p[y][x] = support::get<T>();
            }
        }

        {
            const patch<T> q(p);
            for(unit_t y=p.lower.y;y<=p.upper.y;++y)
            {
                for(unit_t x=p.lower.x;x<=p.upper.x;++x)
                {
                    Y_ASSERT(p[y][x] == q[y][x]);
                }
            }
        }

        {
            patch<T> q(p,area::transpose);
            for(unit_t y=p.lower.y;y<=p.upper.y;++y)
            {
                for(unit_t x=p.lower.x;x<=p.upper.x;++x)
                {
                    Y_ASSERT(p[y][x] == q[x][y]);
                }
            }
        }





    }
}

Y_UTEST(patch)
{

    do_patch<int>();
    do_patch<char>();

}
Y_UTEST_DONE()


