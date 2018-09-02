#include "y/ink/ops/edges.hpp"
#include "y/utest/run.hpp"
#include "y/ink/image.hpp"
#include "y/string/convert.hpp"
#include "y/ink/stencil/delta.hpp"
#include "y/ink/stencil/sobel.hpp"
#include "y/ink/stencil/scharr.hpp"

#include "y/ptr/auto.hpp"
#include "y/ios/ocstream.hpp"
#include "y/ink/color/named-colors.hpp"


using namespace upsylon;
using namespace Ink;
using namespace math;

namespace
{
    static inline
    void find_edges(const  string &label,
                    Edges         &edges,
                    const Stencil<int> &sx,
                    const Stencil<int> &sy,
                    const Blur    *blur,
                    const Pixmap3 &src,
                    Engine        &E)
    {
        ImageIO   &img = Image::instance();
        matrix<int> dx;
        matrix<int> dy;
        sx.build(dx);
        sy.build(dy);
        Filter::Apply(edges.pixels,src,RGBtoFloat,E);
        edges.compute(blur,dx,dy,E);

        Filter::Autoscale(edges.grad,Crux::FloatToFloat, E);

        img.save( label + "-grad.png", edges.grad, 0 );

        img.save( label + "-edges.png", edges, 0 );
        index_to_rgba proc;
        img.save( label + "-blobs.png", edges.blobs, proc, 0);
    }
}

Y_UTEST(edges)
{
    ImageIO   &img = Image::Init();
    Dispatcher par = new concurrent::simd();

    {
        ios::ocstream fp("probe.dat");
        for(int a=0;a<3600;++a)
        {
            const float  theta = (a*numeric<float>::pi)/1800.0f;
            const float  x0    =  cosf(theta);
            const float  y0    =  sinf(theta);
            const unit_t x = unit_t(  floorf( x0 + 0.5f ) );
            const unit_t y = unit_t(  floorf( y0 + 0.5f ) );
            fp("%g %g\n%g %g\n\n", x0,y0,double(x), double(y) );
        }
    }

    DeltaX<int> DX;
    DeltaY<int> DY;

    Sobel3X<int> sobel3X;
    Sobel3Y<int> sobel3Y;

    Sobel5X<int> sobel5X;
    Sobel5Y<int> sobel5Y;

    Scharr3X<int> scharr3X;
    Scharr3Y<int> scharr3Y;

    Scharr5X<int> scharr5X;
    Scharr5Y<int> scharr5Y;

    matrix<int> dx;
    matrix<int> dy;

    DX.build(dx);
    DY.build(dy);

    if(argc>1)
    {
        const Pixmap3  src  = img.load3(argv[1],0);
        auto_ptr<Blur> pBlur = NULL;
        if(argc>2)
        {
            const float r = string_convert::to<float>(argv[2],"r");
            pBlur     = new Blur(r);
        }
        Blur *blur = 0;
        if(pBlur.is_valid()) blur = & *pBlur;
        Engine        E(par,src);
        const size_t  w = src.w;
        const size_t  h = src.h;

        img.save("img.png",src,0);

        Edges edges(w,h);
        find_edges("delta",   edges, DX,       DY     ,blur,src,E);
        find_edges("sobel3",  edges, sobel3X,  sobel3Y,blur,src,E);
        find_edges("sobel5",  edges, sobel5X,  sobel5Y,blur,src,E);
        find_edges("scharr3", edges, scharr3X, scharr3Y,blur,src,E);
        find_edges("scharr5", edges, scharr5X, scharr5Y,blur,src,E);


    }


}
Y_UTEST_DONE()

