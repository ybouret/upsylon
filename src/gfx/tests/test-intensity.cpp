#include "y/gfx/ops/intensity.hpp"
#include "y/utest/run.hpp"
#include "y/gfx/image/io.hpp"
#include "y/gfx/image/io.hpp"
#include "y/concurrent/loop/simt.hpp"
#include "y/concurrent/loop/solo.hpp"
#include "y/gfx/color/convert.hpp"
#include "y/gfx/pixel/blend.hpp"

using namespace upsylon;
using namespace graphic;



static inline rgb rgb2sat(const rgb &c)
{
    const rgb white(255,255,255);
    uint8_t  alpha = 10;
    return blend<uint8_t,rgb>::mix(alpha,white,c);
}

Y_UTEST(intensity)
{
    image::io &IMG       = Y_IMAGE();
    engine     seqEngine = new concurrent::solo();
    engine     parEngine = new concurrent::simt();


    if(argc>1)
    {
        const pixmap<rgb> img = IMG.load<rgb>(argv[1]);
        IMG.save(img,"img.png");
        broker            seq(seqEngine,img);
        broker            par(parEngine,img);

        intensity Iseq,Ipar;
        Iseq.scan(img,seq);std::cerr << "seq: vmin=" << Iseq.vmin << " | vmax=" << Iseq.vmax << std::endl;
        Ipar.scan(img,par);std::cerr << "par: vmin=" << Ipar.vmin << " | vmax=" << Ipar.vmax << std::endl;

        pixmap<float> flt(img,par,convert<float,rgb>::from);
        IMG.save(flt,"flt.png");
        Iseq.scan(flt,seq);std::cerr << "seq: vmin=" << Iseq.vmin << " | vmax=" << Iseq.vmax << std::endl;
        Ipar.scan(flt,par);std::cerr << "par: vmin=" << Ipar.vmin << " | vmax=" << Ipar.vmax << std::endl;

        pixmap<float> pxm(img.w,img.h);
        Iseq.enhance(pxm,flt,seq);
        IMG.save(pxm,"enhf-seq.png");

        Ipar.enhance(pxm,flt,par);
        IMG.save(pxm,"enhf-par.png");

        {
            pixmap<rgb> sat(img,par,pixel::saturated<rgb>);
            IMG.save(sat,"sat.png");
        }

        {
            pixmap<rgb> sat(img,par,rgb2sat);
            IMG.save(sat,"blend.png");
        }


        pixmap<rgb> tgt(img.w,img.h);
        Iseq.scan(img,seq);
        Iseq.enhance(tgt,img,seq,0.7f);
        IMG.save(tgt,"enh3-seq.png");



    }

}
Y_UTEST_DONE()
