
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
#include "y/gfx/color/named.hpp"


using namespace upsylon;
using namespace graphic;



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

        shared_knots   cache = new knots();
        size_to_rgba   conv  = Y_RED_INDEX;

        blobs b(img.w,img.h,cache);
        b.build(fg,4);
        IMG.save(b.bmask,"blobs4.png",NULL,conv);
        std::cerr << "#blobs4: " << b.blist.size << std::endl;
        
        b.build(fg,8);
        IMG.save(b.bmask,"blobs8.png",NULL,conv);
        std::cerr << "#blobs8: " << b.blist.size << std::endl;

    }
    
}
Y_UTEST_DONE()

