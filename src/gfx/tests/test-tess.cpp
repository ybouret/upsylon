#include "y/gfx/area/tiles.hpp"
#include "y/concurrent/loop/simt.hpp"
#include "y/concurrent/loop/solo.hpp"
#include "y/gfx/pixmaps.hpp"

#include "y/utest/run.hpp"
#include "y/gfx/color/random.hpp"

#include "y/gfx/image/io.hpp"

using namespace upsylon;
using namespace graphic;


namespace {

    template <typename T>
    static inline void set_some_color(const tile &t,
                                      void       *args,
                                      lockable   &sync )
    {
        T          tmp(0);
        {
            Y_LOCK(sync);
            tmp = random_color::get<T>(alea);
        }
        pixmap<T> &pxm = *static_cast<pixmap<T>*>(args);
        for(size_t j=t.size();j>0;--j)
        {
            const segment &s  = t[j];
            const unit_t   y  = s.y;
            const unit_t   x0 = s.xmin;
            for(unit_t x=s.xmax;x>=x0;--x)
            {
                pxm[y][x] = tmp;
            }
        }
    }

    template <typename T>
    static inline void set_some_color(pixmap<T>    &pxm,
                                      const engine &loop,
                                      image::io    &IMG)
    {
        broker B(loop,pxm);
        B(set_some_color<T>,&pxm);

        const string filename = string(loop->category()) + ".png";
        IMG.save(pxm,filename);


    }

}

Y_UTEST(tess)
{
    image::io &IMG = Y_IMAGE();

    engine seq_loop = new concurrent::solo();
    engine par_loop = new concurrent::simt();

    coord dims(100,50);
    if(argc>1)
    {
        dims = parsing::wxh(argv[1]);
    }

    pixmap<rgba> pxm(dims.x,dims.y);
    
    set_some_color(pxm,seq_loop,IMG);
    set_some_color(pxm,par_loop,IMG);

    broker        seq(seq_loop,pxm);
    broker        par(par_loop,pxm);

    pixmap<float> seq_gs(seq,pxm, convert<float,rgba>::from);
    pixmap<float> par_gs(par,pxm, convert<float,rgba>::from);

    {
        IMG.save(seq_gs,"seq-gs.png");
        IMG.save(par_gs,"par-gs.png");

    }




}
Y_UTEST_DONE()

