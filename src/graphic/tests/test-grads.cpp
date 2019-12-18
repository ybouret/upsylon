
#include "y/graphic/ops/edges.hpp"

#include "y/utest/run.hpp"
#include "support.hpp"
#include "y/graphic/image.hpp"
#include "y/concurrent/scheme/simd.hpp"
#include "y/graphic/color/ramp/hot_to_cold.hpp"


#include "y/graphic/ops/filter/prewitt.hpp"
#include "y/graphic/ops/filter/sobel.hpp"
#include "y/graphic/ops/filter/scharr.hpp"

using namespace upsylon;
using namespace Graphic;

namespace {

    static inline void doGrads(const Gradients::Pointer &grads,
                               const Pixmap<float>      &source,
                               ForEach                  &par)
    {
        const Ramp::Pointer    ramp = new HotToCold();
        ColorRamp<float>       proc(ramp);
        ColorRamp<uint8_t>     proc1(ramp);

        Tiles tiles( *source, par );

        Edges edges(source->w,source->h);

        Pixmap<float>  &g = edges.g;
        Pixmap<Vertex> &G = edges.G;


        float &gmax = edges.gmax;
        grads->run(g, G, source, tiles, gmax);

        Image &IMG = Image::instance();

        {
            proc.setRange(0, gmax);
            const string saveName = "grad-" + grads->X->key() + "-" + grads->Y->key() + ".png";
            IMG.save(saveName, *g, proc, 0);
        }

        edges.keepLocalMaxima(tiles);
        {
            proc.setRange(0, 255);
            const string saveName = "lmax-" + grads->X->key() + "-" + grads->Y->key() + ".png";
            IMG.save(saveName, *edges.L, proc1, 0);
        }

    }

}


#define DO_GRAD(CLASS) \
do {\
{ const Gradients::Pointer grads = CLASS:: Gradients3(); doGrads(grads,pxm,par); } \
{ const Gradients::Pointer grads = CLASS:: Gradients5(); doGrads(grads,pxm,par); } \
} while(false)


Y_UTEST(grads)
{
    if(argc>1)
    {
        const string        filename = argv[1];
        Image              &IMG      = Image::instance();
        const Pixmap<float> pxm( IMG.loadAs<float>(filename) );
        ForEach             par = new concurrent::simd();

        DO_GRAD(Prewitt);
        DO_GRAD(Sobel);
        DO_GRAD(Scharr);

    }

    


}
Y_UTEST_DONE()

