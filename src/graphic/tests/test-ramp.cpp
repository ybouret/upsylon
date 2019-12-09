#include "y/graphic/color/ramp.hpp"
#include "y/graphic/pixmaps.hpp"
#include "y/graphic/image.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Graphic;

Y_UTEST(ramp)
{

    Image &IMG = Image::instance();

    Pixmap4 pxm4(200,100);


    //IMG.save("ramp.png", *pxm4, <#Data2RGBA &proc#>, NULL);

}
Y_UTEST_DONE()




