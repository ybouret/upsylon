
#include "y/utest/run.hpp"
#include "y/gfx/edges/keep-max.hpp"
#include "y/gfx/edges/profile.hpp"
#include "y/gfx/edges/dvertex-to-rgba.hpp"
#include "y/gfx/color/convert.hpp"
#include "y/gfx/filters/db.hpp"
#include "y/gfx/image/io.hpp"



#include "y/concurrent/loop/env.hpp"
#include "y/gfx/color/size-to-rgba.hpp"
#include "y/gfx/color/ramp/cold-to-hot.hpp"
#include "y/gfx/color/chart.hpp"
#include "y/gfx/ops/gaussian-blur.hpp"
#include "y/gfx/ops/3x3.hpp"

using namespace upsylon;
using namespace graphic;

namespace {

    static inline uint8_t on_off(const uint8_t x)
    {
        return x ? 0xff:0;
    }
}

Y_UTEST(eda)
{

    Y_USE_IMG();
    engine               eng   = concurrent::looper_from_env::get();
    color_ramp           ramp  = new color_tableau<cold_to_hot>();
    color_chart          chart(ramp);
    const shared_filters delta = filters_db::instance()["Sobel3"];
    shared_knots         cache = new knots();
    size_to_rgba         sz2c;

    std::cerr << "engine: " << eng->category() << " : " << eng->size() << std::endl;

    if(argc>1)
    {
        // get image
        const pixmap<rgb>   img = IMG.load<rgb>(argv[1]);
        broker              app(eng,img);

        // convert
        pixmap<float> pxm(img,app,convert<float,rgb>::from);
        IMG.save(img,"img.png");
        IMG.save(pxm,"pxm.png",0,ramp);
        if(argc>2)
        {
            const float          sigma = string_convert::to<float>(argv[2],"sigma");
            gaussian_blur<float> fuzz(7,7,sigma);
            pixmap<float>        src(pxm.w,pxm.h);
            fuzz.cover(src,app,pxm);
            pxm.assign(src,app,identity<float>);
        }

        IMG.save(pxm,"src.png",0,ramp);


        // compute gradient
        edges::gradient grad(img.w,img.h);
        grad.compute(pxm,app,delta);
        IMG.save(grad,"grad.png",0,ramp);

        // keep only maximum parts
        edges::keep_max kmax(img.w,img.h);
        kmax(app,grad);
        IMG.save(kmax,"kmax.png");
        kmax.save("hist.dat");

        {
            pixmap<uint8_t> kbis(kmax,app,on_off);
            IMG.save(kbis,"kbis.png");
        }


        {
            const histogram old_kmax(kmax);
            kmax.update(app);
            Y_CHECK(old_kmax==kmax);
        }

        // get histogram threshold
        const uint8_t  strong = kmax.threshold();
        const uint8_t  feeble = strong>>1;

        // tighten edges
        edges::profile prof;
        prof.tighten(kmax,app,feeble,strong);
        std::cerr << "feeble=" << int(feeble) << " -> strong=" << int(strong) << std::endl;
        IMG.save(kmax,"prof.png");

#if 0
        blobs          B;
        pixmap<size_t> M(img.w,img.h);
        (void) prof.track(B,kmax,M,cache);
        B.sort(M);
        IMG.save(M,"edges.png",0,sz2c);
        IMG.save(kmax,"ekmax.png");

        {
            kmax.close(app);
            IMG.save(kmax,"ckmax.png");

        }
#endif


    }
}
Y_UTEST_DONE()

