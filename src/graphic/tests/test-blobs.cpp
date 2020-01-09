
#include "y/graphic/ops/blobs.hpp"
#include "y/graphic/ops/histogram.hpp"

#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "support.hpp"
#include "y/graphic/image.hpp"
#include "y/concurrent/scheme/simd.hpp"
#include "y/graphic/color/ramp/hot_to_cold2.hpp"
#include "y/graphic/color/named.hpp"
#include "y/ios/ocstream.hpp"

using namespace upsylon;
using namespace Graphic;

Y_UTEST(blobs)
{
    //const Ramp::Pointer    ramp = new HotToCold2();
    //ColorRamp<size_t>      proc(ramp);

    IndexToRGBA proc(Y_RED_INDEX,3);

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

#if 0
            {
                ios::ocstream fp("h2d.dat");
                for(unsigned a=0;a<255;++a)
                {
                    for(unsigned b=a;b<255;++b)
                    {
                        const unsigned a0 = a;
                        const unsigned b0 = b;
                        const unsigned a1 = a;
                        const unsigned b1 = b+1;
                        const unsigned a2 = a+1;
                        const unsigned b2 = b1;

                        const double m0 = H.getMetrics(a0,b0);
                        const double m1 = H.getMetrics(a1,b1);
                        const double m2 = H.getMetrics(a2,b2);

                        fp("%u %u %g\n", a0, b0, m0  );
                        fp("%u %u %g\n", a1, b1, m1  );
                        fp << '\n';
                        fp("%u %u %g\n", a2, b2, m2  );
                        fp("%u %u %g\n", a2, b2, m2  );
                        fp << '\n' << '\n';
                     }
                    //fp << '\n';
                }
            }
#endif

            {
                ios::ocstream fp("otsu.dat");
                for(unsigned i=0;i<255;++i)
                {
                    const double lo = H.getMetrics(0, i);
                    const double hi = H.getMetrics(i+1,255);
                    //std::cerr << "i=" << i << " : " << lo << " " << hi << std::endl;
                    fp("%u %g\n", i, lo+hi );
                }
            }

            {
                ios::ocstream fp("hist.dat");
                for(unsigned i=0;i<=255;++i)
                {
                    fp("%u %g\n", i, double( H[i] ) / pxm->items);
                }
            }

            H.keep(tgt,pxm, t, Convert::Get<uint8_t,float>, KeepForeground, tiles);
            {
                const string saveName = "fgf.png";
                IMG.saveAs(saveName, tgt, 0);
            }

            {
                blobs.build(tgt, Blobs::Connect4,1);
                blobs.rewrite();
                std::cerr << "#blobs_fg_conn4=" << blobs.size << std::endl;
                {
                    const string saveName = "fgf-blobs4.png";
                    IMG.save(saveName, *blobs, proc, 0);
                }
            }

            {
                blobs.build(tgt, Blobs::Connect8,1);
                std::cerr << "#blobs_fg_conn8=" << blobs.size << std::endl;
                {
                    const string saveName = "fgf-blobs8.png";
                    IMG.save(saveName, *blobs, proc, 0);
                }
            }




            H.keep(tgt,pxm, t, Convert::Get<uint8_t,float>, KeepBackground, tiles);
            {
                const string saveName = "bgf.png";
                IMG.saveAs(saveName, tgt, 0);
            }

            {
                blobs.build(tgt, Blobs::Connect4,1);
                std::cerr << "#blobs_bg_conn4=" << blobs.size << std::endl;
                {
                    const string saveName = "bgf-blobs4.png";
                    IMG.save(saveName, *blobs, proc, 0);
                }
            }


            {
                blobs.build(tgt, Blobs::Connect8,1);
                std::cerr << "#blobs_bg_conn8=" << blobs.size << std::endl;
                {
                    const string saveName = "bgf-blobs8.png";
                    IMG.save(saveName, *blobs, proc, 0);
                }
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
