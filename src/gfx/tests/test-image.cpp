

#include "y/gfx/image/io.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace graphic;

Y_UTEST(image)
{
    image::io    &img = Y_IMAGE();
    
    

    if(argc>1)
    {
        const char *filename = argv[1];
        if(img.handles(filename))
        {
            const string _(filename);
            Y_CHECK(img.handles(_));
            pixmap<float> pxm = img.load<float>(filename);
            std::cerr << pxm << std::endl;
            image::options opts("quality=90");
            opts.parse("z=9:alpha=true");
            img.save(pxm,"img-default.jpg");
            img.save(pxm,"img-options.jpg",&opts);
            
            img.save(pxm,"img-default.png");
            img.save(pxm,"img-options.png",&opts);
            
            img.save(pxm,"img-default.tif");
            
        }
        else
        {
            const string _(filename);
            Y_CHECK(!img.handles(_));
        }
    }

}
Y_UTEST_DONE()

