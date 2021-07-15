
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

using namespace upsylon;
using namespace graphic;

namespace {
}

Y_UTEST(eda)
{

    Y_USE_IMG();
    engine               eng   = concurrent::looper_from_env::get();
    color_ramp           ramp  = new color_tableau<cold_to_hot>();
    color_chart          chart(ramp);
    const shared_filters delta = filters_db::instance()["Sobel3"];

    if(argc>1)
    {
        // get image
        const pixmap<rgb>   img = IMG.load<rgb>(argv[1]);
        broker              app(eng,img);

        // convert
        const pixmap<float> pxm(img,app,convert<float,rgb>::from);
        IMG.save(img,"img.png");
        IMG.save(pxm,"pxm.png",0,ramp);

        // compute gradient
        edges::gradient grad(img.w,img.h);
        grad.compute(pxm,app,delta);
        IMG.save(grad,"grad.png",0,ramp);

        // keep only maximum parts
        edges::keep_max kmax(img.w,img.h);
        kmax(app,grad);
        IMG.save(kmax,"kmax.png",0,chart);
        kmax.save("hist.dat");


        //TODO: hist

        const uint8_t  strong = kmax.threshold();
        const uint8_t  feeble = strong>>1;
        edges::profile prof;
        prof.tighten(kmax,app,feeble,strong);

        IMG.save(kmax,"prof.png",0,chart);

        
    }
}
Y_UTEST_DONE()

