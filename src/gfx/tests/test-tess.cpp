

#include "y/gfx/area/tessellation.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/type/spec.hpp"
#include "y/gfx/color/random.hpp"
#include "y/gfx/image/io.hpp"
#include "y/gfx/color/random.hpp"

using namespace upsylon;
using namespace graphic;

namespace
{

}

Y_UTEST(tess)
{
    image::io    &img = Y_IMAGE();

    coord dims(20,10);
    if(argc>1)
    {
        dims = parsing::wxh(argv[1]);
    }
    area        a(dims.x,dims.y);
    pixmap<rgb> pxm(a.w,a.h);
    
    for(size_t cpus=1;cpus<=5;++cpus)
    {
        std::cerr << "<cpus=" << cpus << ">" << std::endl;
        tessellation tess(a,cpus);
        std::cerr << "<cpus=" << cpus << "/>" << std::endl;
        
        const string filename = vformat("tess%u.png",unsigned(cpus));
        for(size_t rank=0;rank<tess.size();++rank)
        {
            const rgb   c = random_color::get<rgba>(alea);
            const tile &t = *tess[rank];
            for(size_t l=t.lines;l>0;--l)
            {
                const segment &s = t[l];
                const unit_t   y = s.y;
                for(unit_t x=s.xmax;x>=s.xmin;--x)
                {
                    pxm[y][x] = c;
                }
            }
        }
        img.save(pxm,filename);
        std::cerr << std::endl;
    }
    
    Y_UTEST_SIZEOF(segment);
    Y_UTEST_SIZEOF(tile);
    Y_UTEST_SIZEOF(tessellation);

}
Y_UTEST_DONE()

