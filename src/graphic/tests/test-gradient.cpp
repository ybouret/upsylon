
#include "y/graphic/ops/gradient.hpp"
#include "y/utest/run.hpp"
#include "y/concurrent/scheme/simd.hpp"
#include "y/graphic/convert.hpp"
#include "y/utest/timings.hpp"

using namespace upsylon;
using namespace Graphic;

namespace {

    template <typename T> static inline
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

#define ALEA_COORD (100+alea.leq(200))


    template <typename T>
    void doGrad(const ForEach &par,
                const ForEach &seq)
    {
        std::cerr << std::endl << "Testing Gradient" << std::endl;
        for(size_t iter=0;iter<4;++iter)
        {
            Pixmap<T> pxm( ALEA_COORD, ALEA_COORD );

            fill(pxm);

            Tiles parTiles(*pxm,par);
            Tiles seqTiles(*pxm,seq);


            Gradient G( pxm->w, pxm->h );

            Ops::Run(parTiles, G.field, pxm, Convert::Get<float,T> );

            double seqSpeed = 0;
            Y_TIMINGS(seqSpeed,1,G.compute(seqTiles));
            std::cerr << "seqSpeed=" << seqSpeed << std::endl;

            double parSpeed = 0;
            Y_TIMINGS(parSpeed,1,G.compute(parTiles));
            std::cerr << "parSpeed=" << parSpeed << std::endl;
        }

    }

}

Y_UTEST(gradient)
{
    ForEach par = new concurrent::simd();
    ForEach seq = new concurrent::sequential_for();
    doGrad<uint8_t>(par,seq);
    doGrad<rgb>(par,seq);
}
Y_UTEST_DONE()


