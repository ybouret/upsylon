#include "y/gfx/ops/gaussian-blur.hpp"
#include "y/gfx/stack.hpp"
#include "y/gfx/pixel.hpp"

#include "y/utest/run.hpp"
#include "y/gfx/image/io.hpp"

#include "y/concurrent/loop/simt.hpp"
#include "y/concurrent/loop/solo.hpp"

using namespace upsylon;
using namespace graphic;

namespace
{

    static float sigma = 2.7f;

    template <typename T> static inline
    void do_blur(const pixmap<rgb> &img,
                 broker            &apply,
                 const char        *sfx)
    {
        static const unit_t max_half = 4;
        static const unit_t num      = 1+max_half;
        static const unit_t count    = num*num;

        const string filename = string("blur") + sfx;
        std::cerr << "<" << filename << ">" << std::endl;
        const pixmap<T> src(img,apply,convert<T,rgb>::from);

        stack<T> stk(num*num,img.w,img.h);

        unit_t idx=0;
        for(unit_t dx=0;dx<num;++dx)
        {
            for(unit_t dy=0;dy<num;++dy)
            {
                pixmap<T>           &tgt = stk[idx++];
                gaussian_blur<float> blr(1+2*dx,1+2*dy,sigma);
                blr.cover(tgt,apply,src);
            }
        }
        assert(count==idx);

        stk.save_tiff(filename,0,count);
        
    }
    
}

Y_UTEST(blur)
{
    Y_USE_IMG();
    

    if(argc>1)
    {
        sigma = string_convert::to<float>(argv[1],"sigma");
    }

    gaussian_blur<float> b(3,5,sigma);

    b.display(std::cerr);
    


    if(argc>2)
    {
        const pixmap<rgb>  img = IMG.load<rgb>(argv[2]);
        engine             seqEngine = new concurrent::solo();
        broker             seq(seqEngine,img);
        
        engine             parEngine = new concurrent::simt();
        broker             par(parEngine,img);
        
        IMG.save(img,"img.png");




        {
            pixmap<rgb> pxm(img.w,img.h);
            
            std::cerr << "blur RGB seq..." << std::endl;
            b.cover(pxm,seq,img);
            IMG.save(pxm,"blur3-seq.png");
            
            std::cerr << "blur RGB par..." << std::endl;
            b.cover(pxm,par,img);
            IMG.save(pxm,"blur3-par.png");

            pixmap<rgb> sub(img.w,img.h);
            sub.assign(img,pxm,seq,pixel::sub<rgb>);
            IMG.save(sub,"sub3-seq.png");

        }
        
        {
            const pixmap<float> imgf(img,seq,convert<float,rgb>::from);
            pixmap<float>       pxmf(img.w,img.h);
            std::cerr << "blur float seq..." << std::endl;
            b.cover(pxmf,seq,imgf);
            IMG.save(pxmf,"blurf-seq.png");
            
            std::cerr << "blur float par..." << std::endl;
            b.cover(pxmf,par,imgf);
            IMG.save(pxmf,"blurf-par.png");
        }
        
        {
            const pixmap<uint8_t> img1(img,seq,convert<uint8_t,rgb>::from);
            pixmap<uint8_t>       pxm1(img.w,img.h);
            std::cerr << "blur byte seq..." << std::endl;
            b.cover(pxm1,seq,img1);
            IMG.save(pxm1,"blur1-seq.png");
            
            std::cerr << "blur byte par..." << std::endl;
            b.cover(pxm1,par,img1);
            IMG.save(pxm1,"blur1-par.png");
        }

        do_blur<rgb>(img,par,"3.tif");
        do_blur<float>(img,par,"f.tif");
        do_blur<uint8_t>(img,par,"1.tif");


    }
    
    
    
}
Y_UTEST_DONE()

