#include "y/ink/ops/filter.hpp"
#include "y/ink/image.hpp"
#include "y/utest/run.hpp"
#include "y/ink/stencil/delta.hpp"
#include "y/ink/stencil/scharr.hpp"
#include "y/ink/stencil/sobel.hpp"
#include "y/ink/ops/channels.hpp"

using namespace upsylon;
using namespace Ink;

namespace
{
    template <typename T>
    static inline
    void display( const Stencil<T> &s )
    {
        matrix<T> M;
        s.build(M);
        std::cerr << s.name << '=' << M << std::endl;
    }
}

Y_UTEST(ops)
{
    ImageIO   &img   = Image::Init();
    Dispatcher __par = new concurrent::simd();

    matrix<int>    M;
    DeltaX<int>    dx;
    DeltaY<int>    dy;
    Laplacian<int> lap;
    Sobel3X<int>   sobel3X;
    Sobel3Y<int>   sobel3Y;
    Sobel5X<int>   sobel5X;
    Sobel5Y<int>   sobel5Y;

    Scharr3X<int>   scharr3X;
    Scharr3Y<int>   scharr3Y;
    Scharr5X<int>   scharr5X;
    Scharr5Y<int>   scharr5Y;

    display(dx);
    display(dy);
    display(lap);

    display(sobel3X);
    display(sobel3Y);
    display(sobel5X);
    display(sobel5Y);

    display(scharr3X);
    display(scharr3Y);
    display(scharr5X);
    display(scharr5Y);

    M.make(5,5);
    M.ld(1);

    for(int iarg=1;iarg<argc;++iarg)
    {
        const string fn  = argv[iarg];
        Pixmap3      pxm = img.load3(fn,NULL);

        const size_t w = pxm.w;
        const size_t h = pxm.h;
        Engine       par(__par,pxm);

        Pixmap1      pxm1(w,h);
        Filter::Apply(pxm1,pxm,RGBtoByte,par);
        img.save("img1.png",pxm1,0);

        PixmapF      pxmf(w,h);
        Filter::Apply(pxmf,pxm,RGBtoFloat,par);
        img.save("imgf.png",pxmf,0);

        {
            Pixmaps<uint8_t> ch(3,w,h);
            Channels::Split(ch,pxm,par);
            red_from_byte   red;
            green_from_byte green;
            blue_from_byte  blue;
            img.save("img_r.png",ch[0],red,NULL);
            img.save("img_g.png",ch[1],green,NULL);
            img.save("img_b.png",ch[2],blue,NULL);
        }

        PixmapF target(w,h);

        {
            Filter::Stencil(target,pxm1,M,par);
            const float vmin = Filter::FindMin(target,par);
            std::cerr << "vmin=" << vmin << std::endl;
            const float vmax = Filter::FindMax(target,par);
            std::cerr << "vmax=" << vmax << std::endl;

            float vmin2=0,vmax2=0;
            Filter::FindMinMax(vmin2,vmax2,target,par);
            std::cerr << "min_max=" << vmin2 << "," << vmax2 << std::endl;
            Y_CHECK(fabsf(vmin-vmin2)<=0);
            Y_CHECK(fabsf(vmax-vmax2)<=0);
            Filter::Rescale(target,vmin,vmax,Crux::FloatToFloat,par);
            img.save("sten1.png",target,0);
        }

        {
            float vmin=0,vmax=0;
            Filter::Stencil(target,pxmf,M,par);
            Filter::FindMinMax(vmin,vmax,target,par);
            std::cerr << "float vmin=" << vmin << ", vmax=" << vmax << std::endl;
            Filter::Rescale(target,vmin,vmax,Crux::FloatToFloat,par);
            img.save("stenf.png",target,0);
        }

#define DO_STENCIL(S) \
do {\
std::cerr << "Stencil " << S.name << std::endl;   \
S.build(M);                                       \
std::cerr << "|_M=" << M << std::endl;            \
Filter::Stencil(target,pxm1,M,par);               \
Filter::Autoscale(target,Crux::FloatToFloat,par); \
img.save( #S ".png",target,0);                    \
} while(false)

        DO_STENCIL(dx);
        DO_STENCIL(dy);
        DO_STENCIL(lap);
        DO_STENCIL(sobel3X);
        DO_STENCIL(sobel3Y);
        DO_STENCIL(sobel5X);
        DO_STENCIL(sobel5Y);

        DO_STENCIL(scharr3X);
        DO_STENCIL(scharr3Y);
        DO_STENCIL(scharr5X);
        DO_STENCIL(scharr5Y);

        



    }

}
Y_UTEST_DONE()

