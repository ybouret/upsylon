#include "y/utest/run.hpp"
#include "y/gfx/edges/profile.hpp"

#include "y/gfx/image/io.hpp"

#include "y/gfx/filters/sobel.hpp"

#include "y/concurrent/loop/simt.hpp"
#include "y/concurrent/loop/solo.hpp"

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

        IMG.save(img,"img.png");

        const shared_filters F = new Sobel5();

        edges::gradient Gpar(img.w,img.h);
        edges::gradient Gseq(img.w,img.h);

        const float     seq_gmax = Gseq.compute(pxm,seq,F);
        const float     par_gmax = Gpar.compute(pxm,par,F);
        Y_CHECK(fabsf(seq_gmax-par_gmax)<=0.0f);
        Y_CHECK(compute_rms(Gseq,Gpar)<=0.0);
        IMG.save(Gpar,"grad.png");


        edges::keep_max Kpar(img.w,img.h);
        edges::keep_max Kseq(img.w,img.h);

        Kseq(seq,Gseq);
        Kpar(par,Gpar);

        Y_CHECK(compute_rms(Kseq,Kpar)<=0);
        Y_CHECK(Kseq==Kpar);
        IMG.save(Kpar,"gmax.png");

        const uint8_t hi = Kseq.threshold();
        const uint8_t lo = hi/2;

        const size_t nseq = edges::profile::tighten(Kseq, seq, lo, hi);
        const size_t npar = edges::profile::tighten(Kpar, par, lo, hi);
        Y_CHECK(compute_rms(Kseq,Kpar)<=0);
        Y_CHECK(nseq==npar);
        Y_CHECK(nseq==Kseq.how_many(seq,pixel::is_not_zero<float>));
        Y_CHECK(npar==Kpar.how_many(seq,pixel::is_not_zero<float>));

        IMG.save(Kpar,"gprf.png");
        std::cerr << "active=" << npar << std::endl;

    }

}
Y_UTEST_DONE()

