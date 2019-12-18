
#include "y/graphic/ops/filter.hpp"
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

        Tiles tiles( *source, par );

        Pixmap<float>  g( source->w, source->h );
        Pixmap<Vertex> G( source->w, source->h);

        Pixmap<float>  Gx( source->w, source->h );
        Pixmap<float>  Gy( source->w, source->h );

        float gmax = 0.0f;
        grads->run(g, G, source, tiles, gmax);

        Image &IMG = Image::instance();

        {
            proc.setRange(0, gmax);
            const string saveName = "grad-" + grads->X->key() + "-" + grads->Y->key() + ".png";
            IMG.save(saveName, *g, proc, 0);
        }

        float GxMin = 0, GxMax = 0;
        float GyMin = 0, GyMax = 0;

        for(unit_t j=0;j<source->h;++j)
        {
            for(unit_t i=0;i<source->w;++i)
            {
                Gx[j][i] = g[j][i] * G[j][i].x; GxMin = min_of(GxMin,Gx[j][i]); GxMax = max_of(GxMax,Gx[j][i]);
                Gy[j][i] = g[j][i] * G[j][i].y; GyMin = min_of(GyMin,Gy[j][i]); GyMax = max_of(GyMax,Gy[j][i]);
            }
        }
        std::cerr << "GxMin=" << GxMin << std::endl;
        std::cerr << "GxMax=" << GxMax << std::endl;

        std::cerr << "GyMin=" << GyMin << std::endl;
        std::cerr << "GyMax=" << GyMax << std::endl;

        {
            {
                proc.setRange(GxMin,GxMax);
                const string saveName = "grad-" + grads->X->key() + ".png";
                IMG.save(saveName, *Gx, proc, 0);
            }

            {
                proc.setRange(GyMin,GyMax);
                const string saveName = "grad-" + grads->Y->key() + ".png";
                IMG.save(saveName, *Gy, proc, 0);
            }

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

