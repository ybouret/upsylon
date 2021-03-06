
#include "y/gfx/ops/hist.hpp"
#include "y/gfx/ops/keep.hpp"

#include "y/gfx/ops/3x3.hpp"
#include "y/gfx/ops/extrema.hpp"
#include "y/gfx/filters/sobel.hpp"
#include "y/gfx/filters/scharr.hpp"
#include "y/gfx/filters/prewitt.hpp"


#include "y/concurrent/loop/simt.hpp"
#include "y/concurrent/loop/solo.hpp"
#include "y/gfx/pixmaps.hpp"

#include "y/utest/run.hpp"
#include "y/utest/timings.hpp"
#include "y/utest/sizeof.hpp"

#include "y/gfx/image/io.hpp"

using namespace upsylon;
using namespace graphic;



Y_UTEST(ops)
{
    image::io &IMG = Y_IMAGE();
    engine seqEngine = new concurrent::solo();
    engine parEngine = new concurrent::simt();

    for(size_t i=0;i<8;++i)
    {
        std::cerr << "delta[" << i << "]=" << area::delta[i] << std::endl;
    }
    
    if(argc>1)
    {
        const pixmap<rgb> img = IMG.load<rgb>(argv[1]);
        IMG.save(img,"img.png");
        std::cerr << "loaded: " << img << std::endl;
        
        
        broker            seq(seqEngine,img);
        broker            par(parEngine,img);
        
        {
            std::cerr << "computing histograms..." << std::endl;
            histogram         Hseq;
            Hseq.reset();
            compute_histogram::update(Hseq,img,seq, convert<uint8_t,rgb>::from );

            histogram Hpar;
            Hpar.reset();
            compute_histogram::update(Hpar,img,par, convert<uint8_t,rgb>::from );

            Hseq.save("hseq.dat");
            Hpar.save("hpar.dat");

            Y_CHECK(Hseq==Hpar);

            if(false)
            {
                double seq_speed = 0;
                Y_TIMINGS(seq_speed,1,compute_histogram::update(Hseq,img,seq, convert<uint8_t,rgb>::from ) );
                std::cerr << "seq_speed=" << seq_speed << std::endl;

                double par_speed = 0;
                Y_TIMINGS(par_speed,1,compute_histogram::update(Hpar,img,par,convert<uint8_t,rgb>::from ) );
                std::cerr << "par_speed=" << par_speed << std::endl;
                std::cerr << "efficiency: " << parEngine->efficiency(par_speed/seq_speed) << std::endl;
            }

            const uint8_t t = Hpar.threshold();
            std::cerr << "threshold=" << int(t) << std::endl;
            pixmap<rgb> fg(img.w,img.h);
            pixmap<rgb> bg(img.w,img.h);

            keep::transfer(fg,img,par,convert<uint8_t,rgb>::from,keep::geq,t,rgb(0,0,0));
            keep::transfer(bg,img,par,convert<uint8_t,rgb>::from,keep::lt,t,rgb(255,255,255));

            IMG.save(fg,"fg.png");
            IMG.save(bg,"bg.png");


            std::cerr << std::endl;
        }


        {
            std::cerr << "computing 3x3 filters" << std::endl;
            pixmap<rgb> tgt(img.w,img.h);

            _3x3::to(tgt,img,seq,pixel::average9<rgb>,convert<rgb,rgb>::from);
            IMG.save(tgt,"ave-seq.png");

            _3x3::to(tgt,img,par,pixel::average9<rgb>,convert<rgb,rgb>::from);
            IMG.save(tgt,"ave-par.png");

            pixmap<float>  gsf(img.w,img.h);
            _3x3::to(gsf,img,par,pixel::average9<rgb>,convert<float,rgb>::from);
            IMG.save(gsf,"ave-gsf.png");

            _3x3::average(tgt,img,par,convert<rgb,rgb>::from); IMG.save(tgt,"average.png");
            _3x3::median(tgt,img,par,convert<rgb,rgb>::from);  IMG.save(tgt,"median.png");
            _3x3::erode(tgt,img,par,convert<rgb,rgb>::from);   IMG.save(tgt,"erode.png");
            _3x3::dilate(tgt,img,par,convert<rgb,rgb>::from);  IMG.save(tgt,"dilate.png");

            pixmap<rgb> tmp(img.w,img.h);
            _3x3::open(tgt,tmp,img,par,identity<rgb>); IMG.save(tgt,"open.png");
            _3x3::close(tgt,tmp,img,par,identity<rgb>); IMG.save(tgt,"close.png");

            std::cerr << std::endl;
        }
        


        {
            std::cerr << "computing extrema" << std::endl;
            const pixmap<float> pxm(img,par,convert<float,rgb>::from);

            const float smax = extrema::get_max(pxm,seq);
            {
                const float pmax = extrema::get_max(pxm,par);
                Y_CHECK( fabsf(smax-pmax)<=0 );
            }

            const float smin = extrema::get_min(pxm,seq);
            {
                const float pmin = extrema::get_min(pxm,par);
                Y_CHECK( fabsf(smin-pmin)<=0 );
            }

            struct minmax_t
            {
                float vmin, vmax;
            };

            minmax_t seq_both = {0,0};
            memcpy(&seq_both,extrema::minmax(pxm,seq),sizeof(minmax_t));

            Y_CHECK( fabsf(seq_both.vmin-smin)<= 0);
            Y_CHECK( fabsf(seq_both.vmax-smax)<= 0);

            minmax_t par_both = { 0,0 };
            memcpy(&par_both,extrema::minmax(pxm,par),sizeof(minmax_t));

            Y_CHECK( fabsf(par_both.vmin-smin)<= 0);
            Y_CHECK( fabsf(par_both.vmax-smax)<= 0);
            std::cerr << std::endl;
        }



    }
}
Y_UTEST_DONE()
