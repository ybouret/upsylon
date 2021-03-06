#include "y/gfx/area/patch.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/type/rtti.hpp"
#include "support.hpp"
#include "y/gfx/color/rgb.hpp"
#include "y/gfx/pblock.hpp"

using namespace upsylon;
using namespace graphic;

namespace {

    template <typename T>
    inline void do_patch()
    {
        std::cerr << "patch<" << rtti::name_of<T>() << ">" << std::endl;


        patch<T> p(alea.range<unit_t>(1,1000), alea.range<unit_t>(1,1000),alea.range<unit_t>(-100,100),alea.range<unit_t>(-100,100));

        std::cerr << "area:" << p << " => " << p.allocated() << " bytes" << std::endl;

        for(unit_t y=p.lower.y;y<=p.upper.y;++y)
        {
            for(unit_t x=p.lower.x;x<=p.upper.x;++x)
            {
                p[y][x] = support::get<T>();
            }
        }

        pblock<T> tmp(p.items);
        
        {
            const patch<T> q(p);
            for(unit_t y=p.lower.y;y<=p.upper.y;++y)
            {
                for(unit_t x=p.lower.x;x<=p.upper.x;++x)
                {
                    Y_ASSERT(p[y][x] == q[y][x]);
                }
            }
            Y_CHECK(p.sum()==q.sum());
            Y_CHECK(p.sum()==p.sum(&tmp[0]));
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
            Y_CHECK(p.sum()==q.sum());
        }





    }
}

Y_UTEST(patch)
{

    do_patch<int>();
    do_patch<char>();

    for(unit_t W=1;W<=1024;++W)
    {
        for(unit_t H=1;H<=1024;++H)
        {
            const coord up = crux::patch::symmetrical_upper(W,H);
            const coord lo = crux::patch::symmetrical_lower(W,H);
            Y_ASSERT(up.x>=0);
            Y_ASSERT(up.y>=0);
            Y_ASSERT(lo.x<=0);
            Y_ASSERT(lo.y<=0);
            Y_ASSERT(W-1==up.x-lo.x);
            Y_ASSERT(H-1==up.y-lo.y);

        }
    }
    
    Y_UTEST_SIZEOF(patch_row<int>);
    Y_UTEST_SIZEOF(patch_row<float>);
    Y_UTEST_SIZEOF(patch_row<uint8_t>);


}
Y_UTEST_DONE()


