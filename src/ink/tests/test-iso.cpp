#include "y/ink/ops/iso.hpp"
#include "y/utest/run.hpp"
#include "y/ink/image.hpp"
#include "y/ink/ops/filter.hpp"
#include "y/ink/color/named-colors.hpp"
#include "y/ink/draw/line.hpp"

using namespace upsylon;
using namespace Ink;
using namespace geometry;

Y_UTEST(iso)
{
    ImageIO   &img   = Image::Init();
    Dispatcher par = new concurrent::simd();

    for(int iarg=1;iarg<argc;++iarg)
    {
        const string fn  = argv[iarg];
        Pixmap3      pxm = img.load3(fn,NULL);

        Iso          iso(pxm);
        Engine       engine(par,iso);

        PixmapF      pxmf(iso.w,iso.h);
        Filter::Apply(pxmf,pxm,RGBtoFloat,engine);

        img.save("imgf.png",pxmf,NULL);
        vector<double> z(3,as_capacity);
        z.push_back(0.25);
        z.push_back(0.50);
        z.push_back(0.75);

        iso.scan(pxmf, z);

        std::cerr << "#levels=" << iso.levels.size() << std::endl;
        for(size_t i=1;i<=iso.levels.size();++i)
        {
            std::cerr << "-- @level=" << z[i] << std::endl;
            const RGB            C      = NamedColor::Fetch(i);
            const Iso2d::Curves &curves = *iso.levels[i];
            std::cerr << "  \\_#curves=" << curves.size() << std::endl;
            for(size_t j=1;j<=curves.size();++j)
            {
                const Iso2d::Curve &curve = *curves[j];
                std::cerr << "    |_curve[" << j << "]: " << curve.size() << std::endl;
                for(size_t k=1;k<curve.size();++k)
                {
                    Draw::Line(pxm, curve[k].x, curve[k].y, curve[k+1].x, curve[k+1].y, C, 200);
                }
            }
        }
        img.save("iso.png",pxm,NULL);
    }
}
Y_UTEST_DONE()

