

#include "y/gfx/filters/sobel.hpp"
#include "y/gfx/filters/prewitt.hpp"
#include "y/gfx/filters/scharr.hpp"

#include "y/gfx/image/io.hpp"
#include "y/gfx/ops/extrema.hpp"

#include "y/concurrent/loop/solo.hpp"
#include "y/concurrent/loop/simt.hpp"

#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"

using namespace upsylon;
using namespace graphic;

namespace
{
    static inline void display_single(const shared_filter &f)
    {
        std::cerr << f->name << ":" << std::endl;
        std::cerr << *f;
    }
    
    static inline void do_filters(const shared_filters &F)
    {
        std::cerr << "<" << F->name << ">" << std::endl;
        display_single(F->y);
        display_single(F->x);
        std::cerr << "<" << F->name << "/>" << std::endl << std::endl;
    }
    
    static inline void do_compute(pixmap<float>       &target,
                                  const pixmap<float> &source,
                                  broker              &apply,
                                  const filter_type   &f)
    {
        f(target,source,apply);
        pixmap<float> tmp(target.w,target.h);
        extrema::normalize(tmp,target,apply);
        const string filename = "img-" + f.name + ".png";
        image::io   &IMG = image::io::location();
        IMG.save(tmp,filename);
    }
    
    static inline void do_filters(const shared_filters &F,
                                  const pixmap<float>  &pxm,
                                  broker               &apply)
    {
        std::cerr << "Computing [" << F->name << "]" << std::endl;
        pixmap<float> fx(pxm.w,pxm.h);
        pixmap<float> fy(pxm.w,pxm.h);
        do_compute(fx,pxm,apply,*(F->x));
        do_compute(fy,pxm,apply,*(F->y));
        
        pixmap<float> fn(pxm.w,pxm.h);
        
        for(unit_t j=0;j<pxm.h;++j)
        {
            for(unit_t i=0;i<pxm.w;++i)
            {
                fn(j)(i) = sqrtf( square_of(fx(j)(i)) + square_of(fy(j)(i))  );
            }
        }
        extrema::normalize(fn, fn, apply);
        image::io   &IMG = image::io::location();
        const string filename = "img-" + F->name + "-grad.png";
        IMG.save(fn,filename);

    }
    
}

Y_UTEST(filters)
{
    const shared_filters sobel3 = new Sobel3();
    const shared_filters sobel5 = new Sobel5();
    const shared_filters sobel7 = new Sobel7();
    
    const shared_filters prewitt3 = new Prewitt3();
    const shared_filters prewitt5 = new Prewitt5();
    const shared_filters prewitt7 = new Prewitt7();

    const shared_filters scharr3 = new Scharr3();
    const shared_filters scharr5 = new Scharr5();

    do_filters(sobel3);
    do_filters(sobel5);
    do_filters(sobel7);
    do_filters(prewitt3);
    do_filters(prewitt5);
    do_filters(prewitt7);
    do_filters(scharr3);
    do_filters(scharr5);

    //engine seqE = new concurrent::solo();
    engine parE = new concurrent::simt();
    
    if(argc>1)
    {
        image::io        &IMG = Y_IMAGE();
        const pixmap<rgb> img = IMG.load<rgb>(argv[1]);
        IMG.save(img,"img.png");
        //broker              seq(seqE,img);
        broker              par(parE,img);
        const pixmap<float> pxm(img,par,convert<float,rgb>::from);
        do_filters(sobel3,pxm,par);
        do_filters(sobel5,pxm,par);
        do_filters(sobel7,pxm,par);
        do_filters(prewitt3,pxm,par);
        do_filters(prewitt5,pxm,par);
        do_filters(prewitt7,pxm,par);
        do_filters(scharr3,pxm,par);
        do_filters(scharr5,pxm,par);

    }
}
Y_UTEST_DONE()

