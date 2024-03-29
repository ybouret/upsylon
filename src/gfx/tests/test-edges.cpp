#include "y/utest/run.hpp"
#include "y/gfx/edges/keep-max.hpp"
#include "y/gfx/edges/profile.hpp"
#include "y/gfx/edges/dvertex-to-rgba.hpp"

#include "y/gfx/image/io.hpp"



#include "y/concurrent/loop/simt.hpp"
#include "y/concurrent/loop/solo.hpp"
#include "y/gfx/color/size-to-rgba.hpp"

using namespace upsylon;
using namespace graphic;

namespace {

    template <typename T>
    static inline double compute_rms(const pixmap<T> &lhs,
                                     const pixmap<T> &rhs)
    {
        double res = 0;
        double rmx = 0;
        coord pos;
        for(unit_t j=0;j<lhs.h;++j)
        {
            for(unit_t i=0;i<lhs.w;++i)
            {
                const double tmp = square_of( double(lhs(j)(i)) - double(rhs(j)(i)) );
                if(tmp>rmx)
                {
                    rmx = tmp;
                    pos = coord(i,j);
                }
                res += tmp;
            }
        }
        return sqrt( res/lhs.items );
    }
}

#include "y/gfx/pixel.hpp"
#include "y/gfx/ops/gaussian-blur.hpp"
#include "y/gfx/filters/db.hpp"
#include "y/gfx/color/named.hpp"

Y_UTEST(edges)
{


    image::io &IMG = Y_IMAGE();
    engine seqEngine = new concurrent::solo();
    engine parEngine = new concurrent::simt();

    if(argc>1)
    {
        const pixmap<rgb>   img = IMG.load<rgb>(argv[1]);
        broker              seq(seqEngine,img);
        broker              par(parEngine,img);
        const pixmap<float> pxm(img,par,convert<float,rgb>::from);
        pixmap<float>       src(img.w,img.h);
        
        if( argc> 2)
        {
            const float          sigma = string_convert::to<float>(argv[2],"sigma");
            gaussian_blur<float> fuzz(7,7,sigma);
            fuzz.cover(src,par,pxm);
            std::cerr << "<blur>" << std::endl;
        }
        else
        {
            std::cerr << "<direct copy>" << std::endl;
            src.assign(pxm,par);
            std::cerr << "<direct copy/>" << std::endl;
        }
            
        
        IMG.save(img,"img.png");
        IMG.save(src,"src.png");

        const shared_filters F = filters_db::instance()["Sobel7"];

        edges::gradient Gpar(img.w,img.h);
        edges::gradient Gseq(img.w,img.h);
        
        const float     seq_gmax = Gseq.compute(src,seq,F);
        const float     par_gmax = Gpar.compute(src,par,F);
        Y_CHECK(fabsf(seq_gmax-par_gmax)<=0.0f);
        Y_CHECK(compute_rms(Gseq,Gpar)<=0.0);
        IMG.save(Gpar,"grad.png");

        edges::dvertex_to_rgba vconv;
        IMG.save(Gpar.probe,"gdir0.png",NULL,vconv);

        edges::keep_max Kpar(img.w,img.h);
        edges::keep_max Kseq(img.w,img.h);

        Kseq(seq,Gseq);
        Kpar(par,Gpar);

        Y_CHECK(compute_rms(Kseq,Kpar)<=0);
        Y_CHECK(Kseq==Kpar);
        IMG.save(Kpar,"gmax.png");
        IMG.save(Gpar.probe,"gdir1.png",NULL,vconv);


        const uint8_t hi = Kseq.threshold();
        const uint8_t lo = hi/2;

        const size_t nseq = edges::profile::tighten(Kseq, seq, lo, hi);
        const size_t npar = edges::profile::tighten(Kpar, par, lo, hi);
        Y_CHECK(compute_rms(Kseq,Kpar)<=0);
        Y_CHECK(nseq==npar);
        Y_CHECK(nseq==Kseq.how_many(seq,pixel::is_not_zero<uint8_t>));
        Y_CHECK(npar==Kpar.how_many(par,pixel::is_not_zero<uint8_t>));

        IMG.save(Kpar,"gprf.png");
        std::cerr << "active=" << npar << std::endl;
        
        
        shared_knots cache = new knots();
        cache->ensure(npar);
        
        size_to_rgba   conv = Y_RED_INDEX;
        edges::profile prof;

        blobs B(img.w,img.h,cache);
        prof.track(B,Kseq);
        
        B.sort();
        
        IMG.save(Kseq, "edges.png");
        IMG.save(B.bmask,"blobs.png",NULL,conv);

        std::cerr << "#blobs: " << B.blist.size << std::endl;
        for(const blob *b=B.blist.head;b;b=b->next)
        {
           // std::cerr << "-> " << b->size << std::endl;
        }

        

    }

}
Y_UTEST_DONE()

