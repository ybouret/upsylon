
#include "y/ink/ops/crop.hpp"
#include "y/ink/image.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Ink;

Y_UTEST(crop)
{
    ImageIO   &img   = Image::Init();

    for(int iarg=1;iarg<argc;++iarg)
    {
        const string  fn  = argv[iarg];
        const Pixmap3 pxm = img.load3(fn,0);
        img.save("org.png",pxm,NULL);
        const size_t  nl  = Crop::Left(pxm);
        std::cerr << "nl=" << nl << std::endl;
        const size_t  nr  = Crop::Right(pxm);
        std::cerr << "nr=" << nr << std::endl;
    }
}
Y_UTEST_DONE()

