
#include "y/gfx/image/io.hpp"
#include "y/gfx/color/convert.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace graphic;

Y_UTEST(sgn)
{
    image::io    &img = Y_IMAGE();


    if(argc>1)
    {
        const string filename = argv[1];
        pixmap<rgb>  pxm = img.load<rgb>(filename);
        std::cerr << pxm << std::endl;
        image::options opts("quality=90");
        opts.parse("z=9:alpha=true");

        for(unit_t j=0;j<pxm.h;++j)
        {
            for(unit_t i=0;i<pxm.w;++i)
            {
                rgb &C = pxm[j][i];
                const float level = convert<float,rgb>::from(C);
                if(level>=0.6)
                {
                    C.r=C.g=C.b=0xff;
                }
            }
        }


        img.save(pxm,"img.png",&opts);
    }

}
Y_UTEST_DONE()
