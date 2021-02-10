

#include "y/gfx/area/tile.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace GFX;

Y_UTEST(tile)
{

    Area a(0,0,17,11);    std::cerr << "area = " << a << std::endl;

    for(size_t size=1;size<=3;++size)
    {
        std::cerr << "<size=" << size << ">" << std::endl;
        for(size_t rank=0; rank<size; ++rank)
        {
            std::cerr << "\trank=" << rank << ":" << std::endl;
            Tile s(a,size,rank);
            std::cerr << "\t" <<s  << std::endl;
        }
        std::cerr << "<size=" << size << "/>" << std::endl << std::endl;
    }


}
Y_UTEST_DONE()

