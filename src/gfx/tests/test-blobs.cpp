
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

#if 0
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
#endif

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

        pixmap<size_t> masks(img.w,img.h);
        shared_knots   cache = new knots();
        size_to_rgba   conv;

        blobs b;
        
        b.build(masks,fg,cache,4);
        IMG.save(masks,"blobs4.png",NULL,conv);
        std::cerr << "#blobs4: " << b.size << std::endl;
        
        b.build(masks,fg,cache,8);
        IMG.save(masks,"blobs8.png",NULL,conv);
        
        std::cerr << "#blobs8: " << b.size << std::endl;



    }
    
}
Y_UTEST_DONE()

