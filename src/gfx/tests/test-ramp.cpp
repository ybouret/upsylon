#include "y/gfx/color/ramp/gradation.hpp"
#include "y/gfx/color/ramp/monochrome.hpp"
#include "y/gfx/color/ramp/cold-to-hot.hpp"

#include "y/utest/run.hpp"
#include "y/gfx/image/io.hpp"
#include "y/concurrent/loop/simt.hpp"
#include "y/gfx/color/named.hpp"

using namespace upsylon;
using namespace graphic;


Y_UTEST(ramp)
{
    Y_USE_IMG();
    engine                  eng = new concurrent::simt();

    monochrome mr( Y_RED );
    color_ramp mono = new color_tableau<monochrome>(mr);
    color_ramp cth  = new color_tableau<cold_to_hot>();

    color_gradation  *g   = new color_gradation();
    color_ramp        grd = g;

    g->push_back(Y_BLUE);
    g->push_back(Y_CYAN);
    g->push_back(Y_GREEN);
    g->push_back(Y_YELLOW);
    g->push_back(Y_RED);


    {
        pixmap<float>       rmp(512,128);
        color_map           cnv(cth,0,rmp.w-1);
        for(unit_t x=0;x<rmp.w;++x)
        {
            for(unit_t y=0;y<rmp.h;++y)
            {
                rmp[y][x] = float(x);
            }
        }
        IMG.save(rmp,"col.png", NULL,cnv);

    }

    if(argc>1)
    {
        const string filename = argv[1];
        const pixmap<rgb> img = IMG.load<rgb>(filename);
        IMG.save(img,"img.png");
        broker            apply(eng,img);
        pixmap<float>     pxm(img,apply,convert<float,rgb>::from);
        IMG.save(pxm,"pxm.png");
        IMG.save(pxm,"mono.png", NULL, mono);
        IMG.save(pxm,"cth.png",  NULL, cth);
        IMG.save(pxm,"grd.png",  NULL, grd);

    }

#if 0
    gradation               g(4);

    g.push_back(Y_BLUE);
    g.push_back(Y_CYAN);
    g.push_back(Y_GREEN);
    g.push_back(Y_YELLOW);
    g.push_back(Y_RED);




    if(argc>1)
    {
        color_ramp::mapping     conv(g);
        const string filename = argv[1];
        const pixmap<rgb> img = IMG.load<rgb>(filename);
        IMG.save(img,"img.png");
        broker            apply(eng,img);
        pixmap<float>     pxm(img,apply,convert<float,rgb>::from);
        IMG.save(pxm,"pxm.png");
        IMG.save(pxm,"rmp.png", NULL,conv);

        {
            monochrome             mh( Y_RED );
            tableau_of<monochrome> mt(mh);
            color_ramp::mapping    mm(mt);
            IMG.save(pxm,"mono.png", NULL,mm);
        }
        {
            tableau_of<cold_to_hot> mt;
            color_ramp::mapping     mm(mt);
            IMG.save(pxm,"cth.png", NULL,mm);
        }




    }
#endif
}
Y_UTEST_DONE()
