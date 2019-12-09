
#include "y/graphic/image/tiff.hpp"

#include "y/utest/run.hpp"
#include "y/graphic/pixmaps.hpp"

using namespace upsylon;
using namespace Graphic;

Y_UTEST(tiff)
{
    TIFF_Format fmt;

    Image::Options options = "quality=80";
    std::cerr << "Options=" << options << std::endl;

    for(int i=1;i<argc;++i)
    {
        const string  filename = argv[i];
        PutRGBA<rgba> put4;
        PutRGBA<rgb>  put3;
        Pixmap<rgba>  pxm4( fmt.load(filename, 4, put4, NULL) );
        Pixmap<rgb>   pxm3( fmt.load(filename, 3, put3, NULL) );

        {
            const string  output = vformat("img%u-4.tiff",i);
            GetRGBA<rgba> get4;
            fmt.save(output, *pxm4, get4, &options);

        }

        {
            const string  output = vformat("img%u-3.tiff",i);
            GetRGBA<rgb> get3;
            fmt.save(output, *pxm3, get3, &options);
        }

    }

}
Y_UTEST_DONE()
