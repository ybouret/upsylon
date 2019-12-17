#include "y/graphic/ops/filter.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include "y/graphic/image.hpp"
#include "y/concurrent/scheme/simd.hpp"
#include "y/graphic/color/ramp/hot_to_cold.hpp"


#include "y/graphic/ops/prewitt.hpp"
#include "y/graphic/ops/sobel.hpp"
#include "y/graphic/ops/scharr.hpp"

using namespace upsylon;
using namespace Graphic;


namespace {

    static inline void applyFilter(Kernel::Filter      &F,
                                   Pixmap<float      > &target,
                                   const Pixmap<float> &source,
                                   Tiles               &tiles,
                                   ColorRamp<float>    &proc)
    {
        Image &IMG = Image::instance();
        F.run(target,source,tiles);
        string filename = F.key() + ".png";

        float vmin=0,vmax=0;
        tiles.globalMinMax(vmin,vmax);

        proc.setRange(vmin, vmax);

        IMG.save(filename, *target, proc, NULL);

    }


}

#define DO_APPLY(F) applyFilter(F,target,source,tiles,proc)
#define DO_IMPL(STRUCT) \
STRUCT::X3 STRUCT##X3; DO_APPLY(STRUCT##X3); \
STRUCT::Y3 STRUCT##Y3; DO_APPLY(STRUCT##Y3);\
STRUCT::X5 STRUCT##X5; \
STRUCT::Y5 STRUCT##Y5

Y_UTEST(filter)
{
    const Ramp::Pointer    ramp = new HotToCold();
    ColorRamp<float>       proc(ramp);


    if(argc>1)
    {
        Image &IMG = Image::instance();
        const string    filename = argv[1];
        Pixmap<float>   source( IMG.loadAs<float>(filename) );
        ForEach         par = new concurrent::simd();
        Tiles           tiles(*source,par);
        Pixmap<float>   target( source->w, source->h );

        DO_IMPL(Prewitt);
        DO_IMPL(Sobel);
        DO_IMPL(Scharr);
    }

}
Y_UTEST_DONE()

