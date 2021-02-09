
#include "y/gfx/bitmap.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace GFX;

Y_UTEST(bitmap)
{

    for(unit_t w=1;w<=1000; w += 10 + alea.leq(100) )
    {
        for(unit_t h=1;h<=1000; h += 10 + alea.leq(100) )
        {

            for(unit_t bpp=1;bpp<=4;++bpp)
            {
                Bitmap bmp(w,h,bpp);
                std::cerr << bmp << std::endl;
                
            }

        }

    }


}
Y_UTEST_DONE()

