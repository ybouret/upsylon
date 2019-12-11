#include "y/graphic/color/ramp/greyscale.hpp"
#include "y/graphic/color/ramp/double_hot.hpp"
#include "y/graphic/color/ramp/hot_to_cold.hpp"
#include "y/graphic/color/ramp/hot_to_cold2.hpp"
#include "y/graphic/color/ramp/orange.hpp"
#include "y/graphic/color/ramp/yellow_to_blue.hpp"

#include "y/graphic/pixmaps.hpp"
#include "y/graphic/image.hpp"
#include "y/utest/run.hpp"



using namespace upsylon;
using namespace Graphic;

namespace {

    void saveRamp( const char *name,
                  Ramp  &ramp )
    {
        Image &IMG = Image::instance();
        Image::Options options = "z=9";

        assert(name);
        string fileName = name;
        fileName += ".png";

        Pixmap4 pxm(256,256);

        for(unit_t j=0;j<pxm->h;++j)
        {
            for(unit_t i=0;i<pxm->w;++i)
            {
                pxm[j][i] = ramp(i*j,0,pxm->items);
            }
        }

        IMG.saveAs(fileName, pxm, &options);


    }
}

#define RAMP(ID) do { ID ramp; saveRamp( #ID, ramp ); } while(false)

Y_UTEST(ramp)
{
    RAMP(Greyscale);
    RAMP(DoubleHot);
    RAMP(HotToCold);
    RAMP(HotToCold2);
    RAMP(Orange);
    RAMP(YellowToBlue);

}
Y_UTEST_DONE()




