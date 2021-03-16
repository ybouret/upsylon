#include "y/utest/run.hpp"
#include "y/gfx/edges/gradient.hpp"
#include "y/gfx/edges/keep-max.hpp"

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

    }

}
Y_UTEST_DONE()

