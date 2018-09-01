#include "y/ink/ops/edges.hpp"
#include "y/utest/run.hpp"
#include "y/ink/image.hpp"
#include "y/string/convert.hpp"
#include "y/ink/stencil/delta.hpp"
#include "y/ptr/auto.hpp"
#include "y/ios/ocstream.hpp"

using namespace upsylon;
using namespace Ink;
using namespace math;

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
        Engine        eng(par,src);
        const size_t  w = src.w;
        const size_t  h = src.h;

        img.save("img.png",src,0);

        Edges edges(w,h);
        Filter::Apply(edges.pixels,src,RGBtoFloat,eng);
        edges.compute(blur,dx,dy,eng);

        Filter::Autoscale(edges.grad,Crux::FloatToFloat,eng);
        img.save("grad.png",edges.grad,0);

        //Filter::Autoscale(edges.border,Crux::FloatToFloat,eng);
        img.save("border.png",edges.border,0);


    }


}
Y_UTEST_DONE()

