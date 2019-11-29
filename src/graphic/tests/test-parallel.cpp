

#include "y/utest/run.hpp"
#include "y/graphic/parallel/copy.hpp"
#include "y/string/convert.hpp"
#include "y/concurrent/scheme/simd.hpp"
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
        const double tmx = 2;

        Pixmap<T> source( ALEA_COORD, ALEA_COORD );
        Tiles     parTiles(*source,par);
        Tiles     seqTiles(*source,seq);
        std::cerr << *source << std::endl;
        std::cerr <<  "par=" << parTiles << std::endl;
        std::cerr <<  "seq=" << seqTiles << std::endl;

        fill(source);
        H.set();
        source->run(H);
        const digest horg = H.md();
        std::cerr << "horg=" << horg << std::endl;
        Pixmap<T> target( source->w, source->h );


        
        target->clear();
        double seqSpeed = 0;
        Y_TIMINGS(seqSpeed,tmx,PixmapCopy(target, source, seqTiles));
        H.set();
        target->run(H);
        const digest hseq = H.md();
        std::cerr << "hseq=" << hseq << std::endl;

        std::cerr << "seqSpeed=" << seqSpeed << std::endl;

        target->clear();
        double parSpeed = 0;
        Y_TIMINGS(parSpeed,tmx,PixmapCopy(target, source, parTiles));
        H.set();
        target->run(H);
        const digest hpar = H.md();
        std::cerr << "hpar=" << hpar << std::endl;
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

    doOps<uint8_t>(par,seq);




}
Y_UTEST_DONE()

