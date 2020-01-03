
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
#include "y/graphic/color/ramp/hot_to_cold2.hpp"

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
        IndexToRGBA         iProc;
        const Ramp::Pointer ramp = new HotToCold2();
        ColorRamp<float>    fProc(ramp);
        ColorRamp<uint8_t>  uProc(ramp);

        Image        &IMG = Image::instance();
        const string  sfx = IMG.tags.of<T>() + ".png";

        edges.load(pxm, blur, tiles);
        {
            const string saveName = "data" + sfx;
            IMG.saveAs( saveName, edges.data, 0 );
        }

        edges.computeGradients(gradients,tiles);
        {
            fProc.setRange(0, edges.gmax);
            const string saveName = "grad" + sfx;
            IMG.save( saveName, *edges.g, fProc, 0 );
        }

        edges.keepLocalMaxima(tiles);
        {
            const string saveName = "lmax" + sfx;
            IMG.saveAs( saveName,edges.L,0);
        }
        
        {
            const string saveName = "hist" + IMG.tags.of<T>() + ".dat";
            ios::ocstream fp(saveName);
            for(unsigned i=0;i<256;++i)
            {
                fp("%u %u\n", i, unsigned(edges.hist[i]) );
            }
        }

        const size_t np = edges.applyThresholds(tiles);
        std::cerr << "#collected=" << np << std::endl;
        {
            const string saveName = "dual" + sfx;
            IMG.save( saveName, *edges.L, uProc, 0);
            //IMG.saveAs( saveName, edges.L, 0);
        }

#if 0
        edges.find(pxm, blur, gradients, tiles);
        std::cerr << "#edges=" << edges.size << std::endl;

        {
            const string saveName = "data" + suffix;
            IMG.saveAs( saveName, edges.data, 0 );
        }

        {
            fProc.setRange(0, edges.gmax);
            const string saveName = "grad" + suffix;
            IMG.save( saveName, *edges.g, fProc, 0 );
        }

        {
            const string saveName = "hist" + IMG.tags.of<T>() + ".dat";
            ios::ocstream fp(saveName);
            for(unsigned i=0;i<256;++i)
            {
                fp("%u %u\n", i, unsigned(edges.hist[i]) );
            }
        }

        {
            const string saveName = "dual" + suffix;
            IMG.saveAs( saveName, edges.L, 0 );
        }

        {
            const string saveName = "edges" + suffix;
            IMG.save(saveName, *edges, iProc, 0);
        }
#endif
    }

}

Y_UTEST(edges)
{
    Image                 &IMG = Image::instance();
    ForEach                par = new concurrent::simd();

    if(argc>1)
    {
        auto_ptr<Blur> blur   = 0;
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

        {
            const Gradients::Pointer gradients = Scharr::Gradients5();
            doEdges( edges, pxm1, tiles, blur.content(), gradients);
            return 0;
            doEdges( edges, pxmF, tiles, blur.content(), gradients);
            doEdges( edges, pxm3, tiles, blur.content(), gradients);
            doEdges( edges, pxm4, tiles, blur.content(), gradients);
        }


    }
    
}
Y_UTEST_DONE()

