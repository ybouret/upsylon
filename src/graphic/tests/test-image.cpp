#include "y/graphic/image.hpp"
#include "y/graphic/pixmap.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Graphic;



Y_UTEST(image)
{
    Image &IMG = Image::instance();


    for(int i=1;i<argc;++i)
    {
        const string         filename = argv[i];

        Pixmap<RGBA>    pxm4( IMG.loadAs<RGBA>(filename) );
        Pixmap<RGB>     pxm3( IMG.loadAs<RGB>(filename) );
        Pixmap<uint8_t> pxm1( IMG.loadAs<uint8_t>(filename) );
        Pixmap<float>   pxmf( IMG.loadAs<float>(filename) );

    }

}
Y_UTEST_DONE()

