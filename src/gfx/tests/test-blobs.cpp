
#include "y/gfx/ops/blobs.hpp"
#include "y/gfx/ops/hist.hpp"
#include "y/gfx/ops/keep.hpp"


#include "y/concurrent/loop/simt.hpp"
#include "y/concurrent/loop/solo.hpp"
#include "y/utest/run.hpp"
#include "y/utest/timings.hpp"
#include "y/utest/sizeof.hpp"

#include "y/gfx/image/io.hpp"
#include "y/gfx/color/size-to-rgba.hpp"


using namespace upsylon;
using namespace graphic;

namespace
{
    template <typename T>
    static inline void display_for(const tiles &tess, const pixmap<T> &pxm)
    {
        std::cerr << "<tessellation #" << tess.size() << ">" << std::endl;
        for(size_t i=0;i<tess.size();++i)
        {
            const tile &t = tess[i];
            std::cerr << "tess[" << std::setw(2) << i << "] : " << t << " => " << blobs::knots_for(t,pxm) << std::endl;
        }
        std::cerr << "<tessellation/>" << std::endl << std::endl;
    }
}

Y_UTEST(blobs)
{

    Y_UTEST_SIZEOF(knot);
    Y_UTEST_SIZEOF(knots);
    Y_UTEST_SIZEOF(blob);


    image::io &IMG       = Y_IMAGE();
    engine     seqEngine = new concurrent::solo();
    engine     parEngine = new concurrent::simt();
    
    
    if(argc>1)
    {
        const pixmap<rgb> img = IMG.load<rgb>(argv[1]);
        IMG.save(img,"img.png");
        std::cerr << "loaded: " << img << std::endl;
        
        broker       seq(seqEngine,img);
        broker       par(parEngine,img);
        pixmap<rgb>  fg(img.w,img.h);
        
        {
            histogram H;
            compute_histogram::update(H,img,par, convert<uint8_t,rgb>::from );
            const uint8_t t = H.threshold();
            keep::transfer(fg,img,par,convert<uint8_t,rgb>::from,keep::geq,t,rgb(0,0,0));
            IMG.save(fg,"fg.png");
        }
        display_for(seq,fg);
        display_for(par,fg);

        shared_marks indices = new marks(img.w,img.h);
        shared_knots k_cache = new knots();
        blobs        b(indices,k_cache);

        b.initialize(seq[0],fg);

        for(unit_t y=0;y<img.h;++y)
        {
            for(unit_t x=0;x<img.w;++x)
            {
                if( !pixel::is_zero( fg(y)(x) ) )
                {
                    (*indices)(y)(x) = x+y;
                }
            }
        }

        {
            size_to_rgba conv;
            IMG.save(*indices,"tags.png", NULL, conv);
        }




        


    }
    
}
Y_UTEST_DONE()

