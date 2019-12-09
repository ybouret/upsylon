#include "y/graphic/image/png.hpp"
#include "y/utest/run.hpp"
#include "y/graphic/pixmaps.hpp"

using namespace upsylon;
using namespace Graphic;

Y_UTEST(png)
{
    PNG_Format PNG;

    Image::Options options = "alpha=true:z=9";
    std::cerr << "Options=" << options << std::endl;
    
    for(int i=1;i<argc;++i)
    {
        const string  filename = argv[i];
        PutRGBA<rgba> put4;
        PutRGBA<rgb>  put3;
        Pixmap<rgba>  pxm4( PNG.load(filename, 4, put4, NULL) );
        Pixmap<rgb>   pxm3( PNG.load(filename, 3, put3, NULL) );

        {
            const string  output = vformat("img%u-4.png",i);
            GetRGBA<rgba> get4;
            PNG.save(output, *pxm4, get4, &options);

        }

        {
            const string  output = vformat("img%u-3.png",i);
            GetRGBA<rgb>  get3;
            PNG.save(output, *pxm3, get3, &options);
        }

    }
}
Y_UTEST_DONE()

