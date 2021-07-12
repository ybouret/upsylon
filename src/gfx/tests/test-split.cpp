#include "y/gfx/image/io.hpp"
#include "y/gfx/ops/split.hpp"
#include "y/concurrent/loop/simt.hpp"
#include "y/gfx/color/named.hpp"
#include "y/gfx/color/ramp/monochrome.hpp"
#include "y/gfx/color/chart.hpp"

#include "y/utest/run.hpp"

using namespace upsylon;
using namespace graphic;

Y_UTEST(split)
{
    const monochrome monoch[4] = { monochrome(Y_RED), monochrome(Y_GREEN), monochrome(Y_BLUE), monochrome(Y_WHITE) };

    Y_USE_IMG();
    engine eng = new concurrent::simt();

    if(argc>1)
    {
        const pixmap<rgba> img = IMG.load<rgba>(argv[1]);
        IMG.save(img,"img.png");
        std::cerr << "loaded: " << img << std::endl;
        broker       apply(eng,img);
        stack<uint8_t> target(4,img.w,img.h);

        const unsigned channels[] = { 0,1,2,3 };
        split(target,img,apply,channels,4);

        for(unsigned i=0;i<4;++i)
        {
            const string      filename = vformat("split%u.png",i);
            const color_ramp  ramp     = new color_tableau<monochrome>(monoch[i]);
            const color_chart chart(ramp);
            
            IMG.save(target[i], filename, NULL, chart);
        }

    }

}
Y_UTEST_DONE()

