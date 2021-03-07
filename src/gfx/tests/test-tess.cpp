

#include "y/gfx/area/tile.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/type/spec.hpp"
#include "y/gfx/color/random.hpp"

using namespace upsylon;
using namespace graphic;

namespace
{

}

Y_UTEST(tess)
{
    coord dims(20,10);
    if(argc>1)
    {
        dims = parsing::wxh(argv[1]);
    }
    area a(dims.x,dims.y);
    
    for(size_t cpus=1;cpus<=4;++cpus)
    {
        std::cerr << "<cpus=" << cpus << ">" << std::endl;
        for(size_t rank=0;rank<cpus;++rank)
        {
            const tile t(a,cpus,rank);
        }
        std::cerr << "<cpus=" << cpus << "/>" << std::endl;
        std::cerr << std::endl;
    }
    
    Y_UTEST_SIZEOF(segment);
    Y_UTEST_SIZEOF(tile);
    
}
Y_UTEST_DONE()

