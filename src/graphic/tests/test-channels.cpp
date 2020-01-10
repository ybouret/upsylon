

#include "y/graphic/ops/split.hpp"
#include "y/graphic/ops/merge.hpp"

#include "y/graphic/stack.hpp"
#include "y/graphic/parallel/ops.hpp"

#include "y/utest/run.hpp"
#include "y/graphic/image.hpp"
#include "y/concurrent/scheme/simd.hpp"
#include "y/graphic/color/ramp/hot_to_cold2.hpp"

#include "y/sequence/vector.hpp"
#include "y/randomized/marsaglia.hpp"

using namespace upsylon;
using namespace Graphic;

namespace {

    typedef randomized::Kiss32 Rand;

    static inline uint8_t uHalf( const uint8_t u ) throw()
    {
        return u >> 1;
    }

    static inline float  fMove( const float f, parallel &ctx ) throw()
    {
        randomized::bits &ran = ctx.get<Rand>();
        return f * (1.0f - 0.2f * ran.to<float>() );
    }

}
Y_UTEST(channels)
{
    Image   &IMG = Image::instance();
    ForEach  par = new concurrent::simd();
    const Ramp::Pointer ramp = new HotToCold2();
    ColorRamp<float>    proc( ramp );

    GetRed<uint8_t>   toRed;
    GetGreen<uint8_t> toGreen;
    GetBlue<uint8_t>  toBlue;

    if(argc>1)
    {
        const string fileName = argv[1];
        Pixmap<rgba> pxm4( IMG.loadAs<rgba>(fileName) );
        const unit_t w = pxm4->w;
        const unit_t h = pxm4->h;

        Tiles tiles( *pxm4, par );
        IMG.saveAs("origin.png", pxm4, 0);

        Pixmap<rgb>     pxm3(w,h);
        Ops::Convert(tiles,pxm3,pxm4);
        Stack<uint8_t> uStack(w,h,4);
        vector<size_t> channels;

        channels << 0 << 1 << 2;

        Split::Run( & uStack.front(), pxm4, channels, tiles);
        

        IMG.save("red4.png",   *uStack[0], toRed,   0 );
        IMG.save("green4.png", *uStack[1], toGreen, 0 );
        IMG.save("blue4.png",  *uStack[2], toBlue,  0 );


        Split::Run( & uStack.front(), pxm3, channels, tiles);


        IMG.save("red3.png",   *uStack[0], toRed,   0 );
        IMG.save("green3.png", *uStack[1], toGreen, 0 );
        IMG.save("blue3.png",  *uStack[2], toBlue,  0 );

        Ops::Run(tiles, uStack[2], uStack[2], uHalf);

        {
            Pixmap<rgba> img4(w,h);
            Merge::Run(img4, & uStack.front(), channels, tiles);
            IMG.saveAs("half-blue.png", img4, 0);
        }


        Pixmap<YUV> yuv(w,h);
        Ops::Convert(tiles, yuv, pxm3);

        Stack<float> fStack(w,h,3);
        Split::Run( & fStack.front(), yuv, channels, tiles );

        proc.setRange(0, 1);
        IMG.save("y.png", *fStack[0], proc, 0);

        proc.setRange( YUV::Umin, YUV::Umax);
        IMG.save("u.png", *fStack[1], proc, 0);

        proc.setRange( YUV::Vmin, YUV::Vmax);
        IMG.save("v.png", *fStack[2], proc, 0);

        for(size_t i=0;i<par->number();++i)
        {
            parallel &ctx = par->engine()[i];
            ctx.build<Rand>().reset(i);
        }

        Ops::RunWithContext(tiles, fStack[0], fStack[0], fMove );
        {
            Pixmap<YUV> imgYUV(w,h);
            Merge::Run(imgYUV, & fStack.front(), channels, tiles);
            Pixmap<rgba> img4(w,h);
            Ops::Convert(tiles,img4,imgYUV);
            IMG.saveAs("noise-y.png", img4, 0);

        }

    }

}
Y_UTEST_DONE()

