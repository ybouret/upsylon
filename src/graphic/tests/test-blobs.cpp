
#include "y/graphic/ops/blobs.hpp"
#include "y/graphic/ops/histogram.hpp"

#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "support.hpp"
#include "y/graphic/image.hpp"
#include "y/concurrent/scheme/simd.hpp"
#include "y/graphic/color/ramp/hot_to_cold2.hpp"

using namespace upsylon;
using namespace Graphic;

Y_UTEST(blobs)
{
    const Ramp::Pointer    ramp = new HotToCold2();
    ColorRamp<size_t>      proc(ramp);

    if(argc>1)
    {
        const string        filename = argv[1];
        Image              &IMG      = Image::instance();
        const Pixmap<float> pxm(  IMG.loadAs<float>(filename) );
        const Pixmap<rgb>   pxm3( IMG.loadAs<rgb>(filename)  );
        ForEach             par = new concurrent::simd();

        IMG.saveAs("org.png", pxm3, 0);

        Tiles     tiles( *pxm, par );
        Histogram H;
        Blobs     blobs( pxm->w, pxm->h );

        {
            Pixmap<float> tgt( pxm->w, pxm->h );
            H.build(pxm, Convert::Get<uint8_t,float>, tiles);
            const uint8_t t = H.Otsu1D();
            std::cerr << "Threshold=" << int(t) << std::endl;

            H.keep(tgt,pxm, t, Convert::Get<uint8_t,float>, KeepForeground, tiles);
            {
                const string saveName = "fgf.png";
                IMG.saveAs(saveName, tgt, 0);
            }
            blobs.build(tgt, Blobs::Connect4);
            std::cerr << "#blobs_fg_conn4=" << blobs.size << std::endl;
            proc.setRange(0, blobs.size);
            {
                const string saveName = "fgf-blobs.png";
                IMG.save(saveName, *blobs, proc, 0);
            }



            H.keep(tgt,pxm, t, Convert::Get<uint8_t,float>, KeepBackground, tiles);
            {
                const string saveName = "bgf.png";
                IMG.saveAs(saveName, tgt, 0);
            }

            

        }

        return 0;

        {
            Pixmap<rgb> tgt( pxm->w, pxm->h );
            H.build(pxm3, Convert::Get<uint8_t,rgb>, tiles);

            const uint8_t t = H.Otsu1D();
            std::cerr << "Threshold=" << int(t) << std::endl;


            H.keep(tgt,pxm3, t, Convert::Get<uint8_t,rgb>, KeepForeground, tiles);
            {
                const string saveName = "fg3.png";
                IMG.saveAs(saveName, tgt, 0);
            }

            H.keep(tgt,pxm3, t, Convert::Get<uint8_t,rgb>, KeepBackground, tiles);
            {
                const string saveName = "bg3.png";
                IMG.saveAs(saveName, tgt, 0);
            }
        }



    }

}
Y_UTEST_DONE()
