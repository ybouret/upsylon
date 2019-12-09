#include "y/graphic/image.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Graphic;

Y_UTEST(image)
{
    Image &IMG = Image::instance();


    for(int i=1;i<argc;++i)
    {
        const string         filename = argv[i];
        const Image::Format &fmt = IMG.FormatFor(filename);
    }

}
Y_UTEST_DONE()

