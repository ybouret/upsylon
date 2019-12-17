#include "y/graphic/ops/filter.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include "y/graphic/image.hpp"
#include "y/concurrent/scheme/simd.hpp"
#include "y/graphic/color/ramp/greyscale.hpp"


#include "y/graphic/ops/prewitt.hpp"
#include "y/graphic/ops/sobel.hpp"
#include "y/graphic/ops/scharr.hpp"

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

#define DO_IMPL(STRUCT) \
STRUCT::X3 STRUCT##X3;\
STRUCT::Y3 STRUCT##Y3;\
STRUCT::X5 STRUCT##X5;\
STRUCT::Y5 STRUCT##Y5

Y_UTEST(filter)
{
    const Ramp::Pointer    ramp = new Greyscale();
    ColorRamp<float>       proc(ramp);

    Filter<uint16_t>   F1("f1",Point(-1,-1),Point(1,1));
    fillField(F1);
    F1.compile();


    Filter<float> F2("f2",Point(-1,-1),Point(1,1));
    fillField(F2);
    F2.compile();

    DO_IMPL(Prewitt);
    DO_IMPL(Sobel);
    DO_IMPL(Scharr);



    Image &IMG = Image::instance();
    if(argc>1)
    {
        const string    filename = argv[1];
        Pixmap<float>   pxm( IMG.loadAs<float>(filename) );
        ForEach         par = new concurrent::simd();
        Tiles           tiles(*pxm,par);
        Pixmap<float>   tgt( pxm->w, pxm->h );
        F1.run(tgt,pxm,tiles);
        float vmin=0,vmax=0;
        tiles.globalMinMax(vmin, vmax);
        proc.setRange(vmin, vmax);
        std::cerr << "vmin=" << vmin << ", vmax=" << vmax << std::endl;
        IMG.saveAs("original.png", pxm, NULL);
        IMG.save("filter1.png", *tgt, proc, NULL);

        F2.run(tgt,pxm,tiles);
        tiles.globalMinMax(vmin, vmax);
        proc.setRange(vmin, vmax);
        std::cerr << "vmin=" << vmin << ", vmax=" << vmax << std::endl;
        IMG.save("filter2.png", *tgt, proc, NULL);

    }

}
Y_UTEST_DONE()

