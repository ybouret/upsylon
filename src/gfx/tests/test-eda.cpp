
#include "y/utest/run.hpp"
#include "y/gfx/edges/keep-max.hpp"
#include "y/gfx/edges/profile.hpp"
#include "y/gfx/edges/dvertex-to-rgba.hpp"

#include "y/gfx/image/io.hpp"



#include "y/concurrent/loop/simt.hpp"
#include "y/concurrent/loop/solo.hpp"
#include "y/gfx/color/size-to-rgba.hpp"

using namespace upsylon;
using namespace graphic;

namespace {
}

Y_UTEST(eda)
{

    Y_USE_IMG();
    engine seqEngine = new concurrent::solo();
    engine parEngine = new concurrent::simt();

    if(argc>1)
    {
        const pixmap<rgb>   img = IMG.load<rgb>(argv[1]);
        broker              seq(seqEngine,img);
        broker              par(parEngine,img);
        const pixmap<float> pxm(img,par,convert<float,rgb>::from);
        pixmap<float>       src(img.w,img.h);

    }
}
Y_UTEST_DONE()

