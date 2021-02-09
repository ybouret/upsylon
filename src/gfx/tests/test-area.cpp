
#include "y/gfx/area.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace GFX;

Y_UTEST(area)
{
    Area a(alea.range<unit_t>(-100,100),
           alea.range<unit_t>(-100,100),
           10,20);      std::cerr << "area   = " << a << std::endl;

    Area c = a.getCore();   std::cerr << "core   = " << c << std::endl;
    Area l = a.getLeft();   std::cerr << "left   = " << l << std::endl; Y_CHECK(l.w==1);
    Area r = a.getRight();  std::cerr << "right  = " << r << std::endl; Y_CHECK(r.w==1);
    Area b = a.getBottom(); std::cerr << "bottom = " << b << std::endl; Y_CHECK(b.h==1);
    Area t = a.getTop();    std::cerr << "top    = " << t << std::endl; Y_CHECK(t.h==1);

    Area bl = a.getBottomLeft();  std::cerr << "bottom_left  = " << bl << std::endl; Y_CHECK(bl.n==1);
    Area br = a.getBottomRight(); std::cerr << "bottom_right = " << br << std::endl; Y_CHECK(br.n==1);

    Area tl = a.getTopLeft();  std::cerr << "top_left  = " << tl << std::endl; Y_CHECK(tl.n==1);
    Area tr = a.getTopRight(); std::cerr << "top_right = " << tr << std::endl; Y_CHECK(tr.n==1);

    Y_CHECK(c.n+l.n+r.n+b.n+t.n+4==a.n);

    for(size_t i=0;i<9;++i)
    {
        Area sub = a.get( Position::At[i] );
        Y_ASSERT(a.owns(sub));
    }

    for(unit_t w=1;w<=100;++w)
    {
        for(unit_t h=1;h<=100;++h)
        {
            for(size_t iter=0;iter<16;++iter)
            {
                Area a(alea.range<unit_t>(-100,100),
                       alea.range<unit_t>(-100,100),
                       w,h);
                Area b = a.rand(alea);
                Y_ASSERT(a.owns(b));
            }
        }
    }



}
Y_UTEST_DONE()

