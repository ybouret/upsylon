#include "y/ink/draw/line.hpp"
#include "y/ink/draw/circle.hpp"
#include "y/ink/color/named-colors.hpp"
#include "y/ink/image.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Ink;

namespace
{
    static inline unit_t gen_unit()
    {
        return unit_t(alea.leq(1000))-50;
    }
}
Y_UTEST(draw)
{
    ImageIO   &img = Image::Init();

    const unit_t w = 300 + alea.leq(300);
    const unit_t h = 200 + alea.leq(200);
    Pixmap<float>   imgf(w,h);
    Pixmap<uint8_t> img1(w,h);
    Pixmap<RGB>     img3(w,h);
    Mask            mask;
    mask.reserve(w*h);
    Pixmap<uint8_t> imgb(w,h);

    for(size_t iter=0;iter<20;++iter)
    {
        const RGB     c3 = NamedColor::Fetch(1+alea.leq(NamedColor::Count));
        const uint8_t c1 = RGBtoByte(c3);
        const float   cf = RGBtoFloat(c3);

        {
            const unit_t x0 = gen_unit();
            const unit_t x1 = gen_unit();
            const unit_t y0 = gen_unit();
            const unit_t y1 = gen_unit();

            Draw::HLine(img3,x0,y0,x1,c3);
            Draw::HLine(img1,x0,y0,x1,c1);
            Draw::HLine(imgf,x0,y0,x1,cf);

            Draw::VLine(img3,x0,y0,y1,c3);
            Draw::VLine(img1,x0,y0,y1,c1);
            Draw::VLine(imgf,x0,y0,y1,cf);

            Draw::HLine(imgb,x0,y0,x1,mask);
            Draw::VLine(imgb,x0,y0,y1,mask);

        }

        {
            const unit_t  x0    = gen_unit();
            const unit_t  x1    = gen_unit();
            const unit_t  y0    = gen_unit();
            const unit_t  y1    = gen_unit();
            const uint8_t alpha = alea.full<uint8_t>();

            Draw::HLine(img3,x0,y0,x1,c3,alpha);
            Draw::HLine(img1,x0,y0,x1,c1,alpha);
            Draw::HLine(imgf,x0,y0,x1,cf,alpha);

            Draw::VLine(img3,x0,y0,y1,c3,alpha);
            Draw::VLine(img1,x0,y0,y1,c1,alpha);
            Draw::VLine(imgf,x0,y0,y1,cf,alpha);

            Draw::HLine(imgb,x0,y0,x1,mask);
            Draw::VLine(imgb,x0,y0,y1,mask);
        }

        {
            const unit_t x0 = gen_unit();
            const unit_t x1 = gen_unit();
            const unit_t y0 = gen_unit();
            const unit_t y1 = gen_unit();

            Draw::Line(img3,x0,y0,x1,y1,c3);
            Draw::Line(img1,x0,y0,x1,y1,c1);
            Draw::Line(imgf,x0,y0,x1,y1,cf);

            Draw::Line(imgb,x0,y0,x1,y1,mask);

        }

        {
            const unit_t x0 = gen_unit();
            const unit_t x1 = gen_unit();
            const unit_t y0 = gen_unit();
            const unit_t y1 = gen_unit();
            const uint8_t alpha = alea.full<uint8_t>();

            Draw::Line(img3,x0,y0,x1,y1,c3,alpha);
            Draw::Line(img1,x0,y0,x1,y1,c1,alpha);
            Draw::Line(imgf,x0,y0,x1,y1,cf,alpha);

            Draw::Line(imgb,x0,y0,x1,y1,mask);

        }

        {
            const unit_t xc = gen_unit();
            const unit_t yc = gen_unit();
            const unit_t r  = unit_t( alea.leq(300) );
            Draw::Circle(img3, xc, yc, r, c3);
            Draw::Circle(img1, xc, yc, r, c1);
            Draw::Circle(imgf, xc, yc, r, cf);

            Draw::Circle(imgb, xc, yc, r, mask);

        }

        {
            const unit_t xc = gen_unit();
            const unit_t yc = gen_unit();
            const unit_t r  = unit_t( alea.leq(300) );
            const uint8_t alpha = alea.full<uint8_t>();
            Draw::Circle(img3, xc, yc, r, c3,alpha);
            Draw::Circle(img1, xc, yc, r, c1,alpha);
            Draw::Circle(imgf, xc, yc, r, cf,alpha);

            Draw::Circle(imgb, xc, yc, r, mask);

        }


        {
            const unit_t xc = gen_unit();
            const unit_t yc = gen_unit();
            const unit_t r  = unit_t( alea.leq(100) );
            Draw::Disk(img3, xc, yc, r, c3);
            Draw::Disk(img1, xc, yc, r, c1);
            Draw::Disk(imgf, xc, yc, r, cf);

            Draw::Disk(imgb, xc, yc, r, mask);

        }

        {
            const unit_t xc = gen_unit();
            const unit_t yc = gen_unit();
            const unit_t r  = unit_t( alea.leq(100) );
            const uint8_t alpha = alea.full<uint8_t>();
            Draw::Disk(img3, xc, yc, r, c3,alpha);
            Draw::Disk(img1, xc, yc, r, c1,alpha);
            Draw::Disk(imgf, xc, yc, r, cf,alpha);

            Draw::Disk(imgb, xc, yc, r, mask);

        }

        {
            const unit_t x0 = gen_unit();
            const unit_t x1 = gen_unit();
            const unit_t y0 = gen_unit();
            const unit_t y1 = gen_unit();

            Draw::Fill(img3,x0,y0,x1,y1,c3);
            Draw::Fill(img1,x0,y0,x1,y1,c1);
            Draw::Fill(imgf,x0,y0,x1,y1,cf);

            Draw::Fill(imgb,x0,y0,x1,y1,mask);

        }

        {
            const unit_t x0 = gen_unit();
            const unit_t x1 = gen_unit();
            const unit_t y0 = gen_unit();
            const unit_t y1 = gen_unit();
            const uint8_t alpha = alea.full<uint8_t>();

            Draw::Fill(img3,x0,y0,x1,y1,c3,alpha);
            Draw::Fill(img1,x0,y0,x1,y1,c1,alpha);
            Draw::Fill(imgf,x0,y0,x1,y1,cf,alpha);

            Draw::Fill(imgb,x0,y0,x1,y1,mask);

        }


    }
    img.save("img3.png",img3,NULL);
    img.save("img1.png",img1,NULL);
    img.save("imgf.png",imgf,NULL);
    std::cerr << "#mask=" << mask.size() << std::endl;
    for(const Vertex *v = mask.head(); v; v=v->next )
    {
        imgb[v->position] = alea.range<uint8_t>(64,255);
    }
    img.save("imgb.png",imgb,NULL);

}
Y_UTEST_DONE()


