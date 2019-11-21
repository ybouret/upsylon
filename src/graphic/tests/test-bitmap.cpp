
#include "y/graphic/bitmap.hpp"
#include "y/utest/run.hpp"
#include "y/ptr/auto.hpp"

using namespace upsylon;
using namespace Graphic;

#define ALEA_COORD (1+alea.leq(100))
Y_UTEST(bitmap)
{

    auto_ptr<Bitmap> bmp1 = Bitmap::Create(ALEA_COORD,ALEA_COORD, 1);
    auto_ptr<Bitmap> bmp2 = Bitmap::Create(ALEA_COORD,ALEA_COORD, 2);
    auto_ptr<Bitmap> bmp3 = Bitmap::Create(ALEA_COORD,ALEA_COORD, 3);
    auto_ptr<Bitmap> bmp4 = Bitmap::Create(ALEA_COORD,ALEA_COORD, 4);

}
Y_UTEST_DONE()

