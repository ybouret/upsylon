#include "y/graphic/image.hpp"
#include "y/graphic/draw/line.hpp"
#include "y/graphic/draw/putpixel.hpp"
#include "y/graphic/color/named.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Graphic;

Y_UTEST(draw)
{
    Image &IMG = Image::instance();

    const unit_t w = 200 + alea.leq(200);
    const unit_t h = 200 + alea.leq(200);

    const unit_t xlo = -w/2;
    const unit_t xhi = w+w/2;
    const unit_t ylo = -h/2;
    const unit_t yhi = h+h/2;


    Pixmap<rgba>  img4(w,h);
    Pixmap<float> imgf(w,h);

    for(size_t iter=0; iter<32; ++iter )
    {
        const unit_t x0  = alea.range<unit_t>(xlo,xhi);
        const unit_t x1  = alea.range<unit_t>(xlo,xhi);
        const unit_t y0  = alea.range<unit_t>(ylo,yhi);
        const unit_t y1  = alea.range<unit_t>(ylo,yhi);
        const size_t idx = 1+alea.lt( Y_NAMED_COLORS );
        const rgba   C   = NamedColors::GetRGBA(idx);
        const float  f   = Convert::Get<float,rgba>(C);

        Draw::HLine(img4, x0, y0, x1, C);
        Draw::HLine(imgf, x0, y0, x1, f);

        Draw::VLine(img4, x0, y0, y1, C);
        Draw::VLine(imgf, x0, y0, y1, f);
    }

    for(size_t iter=0; iter<32; ++iter )
    {
        const unit_t x0  = alea.range<unit_t>(xlo,xhi);
        const unit_t x1  = alea.range<unit_t>(xlo,xhi);
        const unit_t y0  = alea.range<unit_t>(ylo,yhi);
        const unit_t y1  = alea.range<unit_t>(ylo,yhi);
        const size_t idx = 1+alea.lt( Y_NAMED_COLORS );
        const rgba   C      = NamedColors::GetRGBA(idx);
        const float  f      = Convert::Get<float,rgba>(C);
        const uint8_t alpha = alea.leq(255);

        Draw::HLine(img4, x0, y0, x1, C, alpha);
        Draw::HLine(imgf, x0, y0, x1, f, alpha);

        Draw::VLine(img4, x0, y0, y1, C, alpha);
        Draw::VLine(imgf, x0, y0, y1, f, alpha);
    }

    for(size_t iter=0; iter<32; ++iter )
    {
        const unit_t x0  = alea.range<unit_t>(xlo,xhi);
        const unit_t x1  = alea.range<unit_t>(xlo,xhi);
        const unit_t y0  = alea.range<unit_t>(ylo,yhi);
        const unit_t y1  = alea.range<unit_t>(ylo,yhi);
        const size_t idx = 1+alea.lt( Y_NAMED_COLORS );
        const rgba   C   = NamedColors::GetRGBA(idx);
        const float  f   = Convert::Get<float,rgba>(C);

        Draw::Line(img4, x0, y0, x1, y1, C);
        Draw::Line(imgf, x0, y0, x1, y1, f);

    }

    for(size_t iter=0; iter<32; ++iter )
    {
        const unit_t x0  = alea.range<unit_t>(xlo,xhi);
        const unit_t x1  = alea.range<unit_t>(xlo,xhi);
        const unit_t y0  = alea.range<unit_t>(ylo,yhi);
        const unit_t y1  = alea.range<unit_t>(ylo,yhi);
        const size_t idx = 1+alea.lt( Y_NAMED_COLORS );
        const rgba   C   = NamedColors::GetRGBA(idx);
        const float  f   = Convert::Get<float,rgba>(C);
        const uint8_t alpha = alea.leq(255);

        Draw::Line(img4, x0, y0, x1, y1, C, alpha);
        Draw::Line(imgf, x0, y0, x1, y1, f, alpha);

    }






    IMG.saveAs( "draw4.png", img4, 0 );
    IMG.saveAs( "drawf.png", imgf, 0 );


}
Y_UTEST_DONE()


