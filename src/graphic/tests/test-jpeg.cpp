#include "y/graphic/image/jpeg.hpp"

#include "y/utest/run.hpp"
#include "y/graphic/pixmaps.hpp"

using namespace upsylon;
using namespace Graphic;

Y_UTEST(jpeg)
{
#if 0
    PNG_Format PNG;

    Image::Options options = "alpha=true:z=9";
    std::cerr << "Options=" << options << std::endl;

    for(int i=1;i<argc;++i)
    {
        const string  filename = argv[i];
        PutRGBA<RGBA> put4;
        PutRGBA<RGB>  put3;
        Pixmap<RGBA>  pxm4( PNG.load(filename, 4, put4, NULL) );
        Pixmap<RGB>   pxm3( PNG.load(filename, 3, put3, NULL) );

        {
            const string  output = vformat("img%u-4.jpg",i);
            GetRGBA<RGBA> get4;
            PNG.save(output, *pxm4, get4, &options);

        }

        {
            const string  output = vformat("img%u-3.jpg",i);
            GetRGBA<RGB> get3;
            PNG.save(output, *pxm3, get3, &options);
        }

    }
#endif

}
Y_UTEST_DONE()
