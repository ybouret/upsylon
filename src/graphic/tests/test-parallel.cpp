

#include "y/utest/run.hpp"
#include "y/string/convert.hpp"
#include "y/graphic/convert.hpp"

#include "y/concurrent/scheme/simd.hpp"
#include "y/graphic/parallel/ops.hpp"
#include "y/graphic/pixmaps.hpp"
#include "y/hashing/sha1.hpp"
#include "y/utest/timings.hpp"

using namespace upsylon;
using namespace Graphic;

#define ALEA_COORD (100+alea.leq(200))


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

    template <typename T> static inline
    void doCpy()
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


    template <typename T, typename U> static inline
    void doConv(const ForEach &par,
                const ForEach &seq)
    {
        std::cerr << "doConv<" << typeid(T).name() << "," << typeid(U).name() << ">" << std::endl;
        const double tmx = 1;

        Pixmap<T> source(ALEA_COORD,ALEA_COORD);
        Pixmap<U> target(source->w,source->h);

        fill(source);

        Tiles parTiles(*source,par);
        Tiles seqTiles(*source,seq);

        double seqSpeed = 0;
        Y_TIMINGS(seqSpeed,tmx, Ops::Run(seqTiles, target, source, Convert::Get<U,T> ) );
        std::cerr << "seqSpeed=" << seqSpeed << std::endl;

        double parSpeed = 0;
        Y_TIMINGS(parSpeed,tmx, Ops::Run(parTiles, target, source, Convert::Get<U,T> ) );
        std::cerr << "parSpeed=" << parSpeed << std::endl;

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

    doCpy<uint8_t>();
    doCpy<float>();
    doCpy<RGB>();
    doCpy<RGBA>();
    
    doConv<uint8_t,float>(par,seq);
    doConv<float,uint8_t>(par,seq);
    doConv<RGB,uint8_t>(par,seq);
    doConv<RGBA,uint8_t>(par,seq);
    doConv<uint8_t,RGB>(par,seq);
    doConv<uint8_t,RGBA>(par,seq);


}
Y_UTEST_DONE()

