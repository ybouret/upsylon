
#include "y/ink/image.hpp"
#include "y/utest/run.hpp"
#include "y/ink/ops/filter.hpp"
#include "y/ptr/arc.hpp"

using namespace upsylon;
using namespace Ink;

namespace
{
    template <typename T>
    static inline void do_sub( Pixmap<T> &pxm, const char id )
    {
        std::cerr << std::endl;
        std::cerr << "DoSub for " << id << ", depth=" << pxm.depth << ", Area=" << pxm << std::endl;
        Tiles tiles(pxm,4);
        for(size_t i=1;i<=tiles.size();++i)
        {
        }
    }
}

Y_UTEST(sub)
{
    Image     &__img = Image::Init();
    ImageIO   &img   = __img;
    Dispatcher par = new concurrent::simd();
    put_rgb    ioRGB;

    for(int iarg=1;iarg<argc;++iarg)
    {
        const string fn = argv[iarg];
        Bitmap      *bmp = img.load(fn,sizeof(RGB),ioRGB,NULL);
        const arc_ptr<Bitmap> guard = bmp;
        Pixmap3      img3( bmp );
        Engine       eng(par,img3);
        const size_t w = img3.w;
        const size_t h = img3.h;
        Pixmap1      img1(w,h);
        PixmapF      imgF(w,h);
        Filter::Apply(img1,img3,RGBtoByte,eng);
        Filter::Apply(imgF,img3,RGBtoFloat,eng);

        do_sub(img3,'3');
        do_sub(img1,'1');
        do_sub(imgF,'F');
    }
}
Y_UTEST_DONE()
