#include "y/graphic/stack.hpp"
#include "y/graphic/draw/circle.hpp"
#include "y/graphic/color/named.hpp"
#include "y/graphic/image.hpp"
#include "y/graphic/parallel/ops.hpp"
#include "y/concurrent/scheme/simd.hpp"

#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Graphic;

Y_UTEST(stack)
{

    Image &IMG = Image::instance();

    const size_t n = 3;
    const unit_t w = 200 + alea.leq(200);
    const unit_t h = 200 + alea.leq(200);

    Stack<float> pxms(w,h,n); Y_CHECK( pxms.count == n );
    Stack<rgb>   rgbs(w,h,n); Y_CHECK( rgbs.count == n );

    for(size_t i=0;i<pxms.count;++i)
    {
        Y_ASSERT(pxms[i]->sameAreaThan(pxms));
    }


    for(size_t i=0;i<n;++i)
    {
        for(size_t iter=0;iter<4;++iter)
        {
            const unit_t  x     = alea.range<unit_t>(0,w);
            const unit_t  y     = alea.range<unit_t>(0,h);
            const unit_t  r     = alea.leq(100);
            const size_t  idx   = 1+alea.lt( Y_NAMED_COLORS );
            const rgb     C     = NamedColors::GetRGBA(idx);
            const float   f     = Convert::Get<float,rgb>(C);
            const uint8_t alpha = alea.range<uint8_t>(10,255);

            Draw::Disk( rgbs[i], x, y, r, C, alpha);
            Draw::Disk( pxms[i], x, y, r, f, alpha);
        }

    }

    pxms.saveTIFF("pxms.tif", 0, n);
    Y_CHECK( I_TIFF::CountDirectoriesOf("pxms.tif") == n );

    {
        auto_ptr< Stack<float> > s = Stack<float>::LoadTIFF("pxms.tif");
        Y_CHECK( s->size() == pxms.size() );
        s->saveTIFF("pxms-reload.tif", 0, n);
    }
    
    rgbs.saveTIFF("rgbs.tif", 0, n);
    Y_CHECK( I_TIFF::CountDirectoriesOf("rgbs.tif") == n );

    {
        auto_ptr< Stack<rgb> > s = Stack<rgb>::LoadTIFF("rgbs.tif");
        Y_CHECK( s->size() == pxms.size() );
        s->saveTIFF("rgbs-reload.tif", 0, n);
    }
    
    {
        Pixmap<float> tgt(w,h);
        pxms.average(tgt);
        IMG.saveAs("averagef.tif", tgt, 0);
    }

    {
        Pixmap<rgb> tgt(w,h);
        rgbs.average(tgt);
        IMG.saveAs("average3.tif", tgt, 0);
    }


    {
        Pixmap<float> df(w,h);
        Pixmap<float> d3(w,h);

        ForEach       par = new concurrent::simd();
        Tiles         tiles(*df,par);

        Ops::Diff(tiles, df, pxms[0], pxms[1] );
        IMG.saveAs("deltaf.png", df, 0);

        Ops::Diff(tiles, d3, rgbs[0], rgbs[1] );
        IMG.saveAs("delta3.png", d3, 0);

        Ops::Diff(tiles,df,df,d3);
        IMG.saveAs("ddelta.png", df, 0);


    }


}
Y_UTEST_DONE()


