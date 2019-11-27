

#include "y/utest/run.hpp"
#include "y/graphic/parallel/tiling.hpp"
#include "y/string/convert.hpp"

using namespace upsylon;
using namespace Graphic;

Y_UTEST(parallel)
{

    size_t CPUs = 4;
    if(argc>1)
    {
        CPUs = string_convert::to<size_t>( argv[1], "CPUs" );
    }
    
    for(size_t W=1;W<=8;++W)
    {

        for(size_t H=1;H<=8;++H)
        {
            const Area  area(W,H);
            std::cerr << "Area: " << area << std::endl;
            Tiling::ComputeTiles(area, CPUs);
        }
    }


}
Y_UTEST_DONE()

