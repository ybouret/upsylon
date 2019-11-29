

#include "y/utest/run.hpp"
#include "y/string/convert.hpp"
#include "y/concurrent/scheme/simd.hpp"
#include "y/graphic/parallel/tiles.hpp"
#include "y/graphic/pixmaps.hpp"
#include "y/hashing/sha1.hpp"
#include "y/utest/timings.hpp"

using namespace upsylon;
using namespace Graphic;

#define ALEA_COORD (1+alea.leq(200))


namespace {

    template <typename T>
    void fill( Pixmap<T> &pxm )
    {
        for(unit_t j=0;j<pxm->h;++j)
        {
            for(unit_t i=0;i<pxm->w;++i)
            {
                alea.fill( &pxm[j][i], pxm->depth);
            }
        }
    }

    template <typename T>
    void doOps(const ForEach &par,
               const ForEach &seq  )
    {
        hashing::sha1 H;

        Pixmap<T> source(ALEA_COORD,ALEA_COORD);

        fill(source);
        const digest hOrg = source->md(H);
        std::cerr << "hOrg=" << hOrg << std::endl;

        Pixmap<T> target(source->w,source->h);

        target.copy(source);
        const digest hCpy = target->md(H);
        std::cerr << "hCpy=" << hCpy << std::endl;
        Y_CHECK(hOrg==hCpy);
    }

}

Y_UTEST(parallel)
{

    ForEach par = new concurrent::simd();
    ForEach seq = new concurrent::sequential_for();


    for(size_t W=1;W<=8;++W)
    {

        for(size_t H=1;H<=8;++H)
        {
            const Area  area(W,H);
            std::cerr << "Area: " << area << std::endl;
            Tiles parTiles(area,par);
            Tiles seqTiles(area,seq);
            std::cerr <<  parTiles << std::endl;
        }
    }

    doOps<uint8_t>(par,seq);




}
Y_UTEST_DONE()

