

#include "y/gfx/image/io.hpp"
#include "y/utest/run.hpp"
#include "y/type/spec.hpp"

using namespace upsylon;
using namespace graphic;

Y_UTEST(image)
{
    image::io    &img = image::io::instance();

    

    img.standard();

    {
        image::named_format *jpeg = img("JPEG"); Y_CHECK(jpeg);
        std::cerr << "jpeg: " << jpeg->extension_lowercase_regexp << std::endl;
        jpeg->extension_compiled_pattern->graphViz("jpeg.dot");
    }

    {
        image::named_format *png = img("PNG"); Y_CHECK(png);
        std::cerr << "png: " << png->extension_lowercase_regexp << std::endl;
        png->extension_compiled_pattern->graphViz("png.dot");
    }


    if(argc>1)
    {
        const char *filename = argv[1];
        if(img.handles(filename))
        {
            const string _(filename);
            Y_CHECK(img.handles(_));
            pixmap<float>   pxm = img.load<float>(filename);
            std::cerr << pxm << std::endl;
            img.save(pxm,"img.jpg");
        }
        else
        {
            const string _(filename);
            Y_CHECK(!img.handles(_));
        }
    }

}
Y_UTEST_DONE()

