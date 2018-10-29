#include "y/ink/draw/line.hpp"
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
    for(size_t iter=0;iter<100;++iter)
    {
        const RGB     c3 = NamedColor::Fetch(1+alea.leq(NamedColor::Count));
        const uint8_t c1 = RGBtoByte(c3);
        const float   cf = RGBtoFloat(c3);

        {
            const unit_t x0 = gen_unit();
            const unit_t x1 = gen_unit();
            const unit_t y0 = gen_unit();

            Draw::HLine(img3,x0,y0,x1,c3);
            Draw::HLine(img1,x0,y0,x1,c1);
            Draw::HLine(imgf,x0,y0,x1,cf);
        }

        {
            const unit_t  x0    = gen_unit();
            const unit_t  x1    = gen_unit();
            const unit_t  y0    = gen_unit();
            const uint8_t alpha = alea.full<uint8_t>();
            //std::cerr << "alpha=" << int(alpha) << std::endl;
            Draw::HLine(img3,x0,y0,x1,c3,alpha);
            Draw::HLine(img1,x0,y0,x1,c1,alpha);
            Draw::HLine(imgf,x0,y0,x1,cf,alpha);
        }


    }
    img.save("img3.png",img3,NULL);
    img.save("img1.png",img1,NULL);
    img.save("imgf.png",imgf,NULL);

}
Y_UTEST_DONE()


