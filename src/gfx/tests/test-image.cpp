

#include "y/gfx/image/jpeg.hpp"
#include "y/gfx/image/io.hpp"
#include "y/utest/run.hpp"
#include "y/type/spec.hpp"

using namespace upsylon;
using namespace graphic;

Y_UTEST(image)
{
    image::io    &img = image::io::instance();

    jpeg_format *jpeg= new jpeg_format();
    img.define(jpeg);

    img.standard();

    std::cerr << "jpeg.rx=" << jpeg->extension_lowercase_regexp << std::endl;
    jpeg->extension_compiled_pattern->graphViz("jpeg.dot");

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

