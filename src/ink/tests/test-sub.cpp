
#include "y/ink/image.hpp"
#include "y/utest/run.hpp"
#include "y/ink/ops/filter.hpp"
#include "y/ptr/arc.hpp"

using namespace upsylon;
using namespace Ink;

namespace
{
    template <typename T>
    static inline void do_sub( Bitmap &bmp, const char id)
    {
        ImageIO   &img = Image::instance();
        std::cerr << std::endl;
        std::cerr << "DoSub for " << id << ", depth=" << bmp.depth << ", Area=" << bmp << std::endl;
        Tiles tiles(bmp,4);
        Pixmap<T>  pxm( &bmp );
        img.save( vformat("full%c.png",id),pxm,NULL);
        for(size_t i=1;i<=tiles.size();++i)
        {
            const Area &A = *tiles[i];
            Pixmap<T>   P( &bmp, A );
            img.save( vformat("full%c-%u.png",id,unsigned(i)), P, NULL);
        }
    }
}

Y_UTEST(sub)
{
    Image     &__img = Image::Init();
    ImageIO   &img   = __img;
    Dispatcher par = new concurrent::simd();
    put_rgb    io3;
    put_gs1    io1;
    put_gsf    ioF;

    for(int iarg=1;iarg<argc;++iarg)
    {
        const string    fn   = argv[iarg];
        arc_ptr<Bitmap> bmp3 = img.load(fn,sizeof(RGB),    io3,NULL);
        arc_ptr<Bitmap> bmp1 = img.load(fn,sizeof(uint8_t),io1,NULL);
        arc_ptr<Bitmap> bmpF = img.load(fn,sizeof(float),  ioF,NULL);


        do_sub<RGB>(*bmp3,'3');
        do_sub<uint8_t>(*bmp1,'1');
        do_sub<float>(*bmpF,'F');

    }
}
Y_UTEST_DONE()
