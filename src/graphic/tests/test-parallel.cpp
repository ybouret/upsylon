

#include "y/utest/run.hpp"
#include "y/graphic/parallel/tiles.hpp"
#include "y/string/convert.hpp"
#include "y/concurrent/scheme/simd.hpp"

using namespace upsylon;
using namespace Graphic;

Y_UTEST(parallel)
{

    concurrent::simd           par;
    concurrent::sequential_for seq;


    for(size_t W=1;W<=8;++W)
    {

        for(size_t H=1;H<=8;++H)
        {
            const Area  area(W,H);
            std::cerr << "Area: " << area << std::endl;
            Tiles parTiles(area,par);
            Tiles seqTiles(area,seq);
            std::cerr << parTiles << std::endl;

            //Tiling::ComputeTiles(area, CPUs);
        }
    }


}
Y_UTEST_DONE()

