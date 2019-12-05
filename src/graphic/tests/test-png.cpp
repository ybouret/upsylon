#include "y/graphic/image/png.hpp"
#include "y/utest/run.hpp"
#include "y/graphic/pixmaps.hpp"

using namespace upsylon;
using namespace Graphic;

Y_UTEST(png)
{
    PNG_Format PNG;
    for(int i=1;i<argc;++i)
    {
        const string  filename = argv[i];
        PutRGBA<RGBA> id4;
        PutRGBA<RGB>  id3;
        Pixmap<RGBA>  pxm4( PNG.load(filename, 4, id4, NULL) );
        Pixmap<RGB>   pxm3( PNG.load(filename, 3, id3, NULL) );
    }
}
Y_UTEST_DONE()

