
#include "y/graphic/ops/edges.hpp"

#include "y/utest/run.hpp"
#include "y/graphic/image.hpp"
#include "y/concurrent/scheme/simd.hpp"


#include "y/graphic/ops/filter/prewitt.hpp"
#include "y/graphic/ops/filter/sobel.hpp"
#include "y/graphic/ops/filter/scharr.hpp"
#include "y/graphic/color/named.hpp"
#include "y/ios/ocstream.hpp"
#include "y/graphic/parallel/ops.hpp"


using namespace upsylon;
using namespace Graphic;

namespace {

    template <typename T>
    static inline
    void doEdges(Edges                    &edges,
                 const Pixmap<T>          &pxm,
                 Tiles                    &tiles,
                 const Blur               *blur,
                 const Gradients::Pointer &gradients)
    {
        edges.find(pxm, blur, gradients, tiles);
    }

}

Y_UTEST(edges)
{
    Image                 &IMG = Image::instance();
    ForEach                par = new concurrent::simd();

    if(argc>1)
    {
        auto_ptr<Blur> blur = 0;
        float          radius = 2.0f;
        if(argc>2)
        {
            radius = string_convert::to<float>( argv[2], "radius" );
        }

        if(radius>0) blur = new Blur(radius);

        const string        filename = argv[1];
        Pixmap<rgba>        pxm4( IMG.loadAs<rgba>(filename) );
        Tiles               tiles( *pxm4, par);
        const unit_t        w = pxm4->w;
        const unit_t        h = pxm4->h;
        Pixmap<rgb>         pxm3(w,h);
        Pixmap<float>       pxmF(w,h);
        Pixmap<uint8_t>     pxm1(w,h);

        Ops::Convert(tiles, pxm3, pxm4);
        Ops::Convert(tiles, pxmF, pxm4);
        Ops::Convert(tiles, pxm1, pxm4);

        Edges edges(w,h);

        const Gradients::Pointer gradients = Scharr::Gradients5();

        doEdges( edges, pxm1, tiles, blur.content(), gradients);

    }
    
}
Y_UTEST_DONE()

