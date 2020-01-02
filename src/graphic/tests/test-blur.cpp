
#include "y/graphic/ops/blur.hpp"

#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "support.hpp"
#include "y/graphic/image.hpp"
#include "y/graphic/pixmaps.hpp"
#include "y/concurrent/scheme/simd.hpp"
#include "y/graphic/color/ramp/hot_to_cold2.hpp"

using namespace upsylon;
using namespace Graphic;

Y_UTEST(blur)
{
    Image                 &IMG  = Image::instance();
    const Ramp::Pointer    ramp = new HotToCold2();
    ColorRamp<float>       proc(ramp);
    ForEach                par = new concurrent::simd();

    if(false)
    {
        for(float sigma=0.25f;sigma<=4.0f;sigma+=0.25f)
        {
            Blur B(sigma);
            std::cerr << "sigma=" << sigma << " => " << B.delta << std::endl;
            const string filename = vformat( "blur%.2f.png",sigma);
            const float  vmax     = B[0][0];
            const float  vmin     = B[B->upper];
            std::cerr << "\tvmin=" << vmin << std::endl;
            std::cerr << "\tvmax=" << vmax << std::endl;
            proc.setRange(vmin,vmax);
            IMG.save(filename, *B, proc, 0);
        }
    }

    if(argc>1)
    {
        const string        filename = argv[1];
        const Pixmap4       pxm4( IMG.loadAs<rgba>(filename) );
        const PixmapF       pxmf( IMG.loadAs<float>(filename) );

        Tiles tiles( *pxmf, par );
        Blur  blur( 2.2f );

        {
            PixmapF tgt(pxmf->w,pxmf->h);
            blur.apply<float,float,1>(tgt,pxmf,tiles);
            IMG.saveAs("origf.png",pxmf,NULL);
            IMG.saveAs("blurf.png",tgt, NULL);
        }


    }
}
Y_UTEST_DONE()
