#include "y/gfx/area/tiles.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace GFX;



Y_UTEST(tiles)
{

    unit_t w = 10;
    unit_t h = 20;

    if(argc>1)
    {
        const Point p = Parse::WxH(argv[1]);
        w=p.x;
        h=p.y;
    }

    Area area(0,0,w,h);
    for(size_t cpus=1;cpus<=8;++cpus)
    {
        std::cerr << "<cpus=" << cpus << ">" << std::endl;
        const Tiles tiles(area,cpus);
        std::cerr << "count=" << tiles.count << std::endl;
        std::cerr << "<cpus=" << cpus << "/>" << std::endl << std::endl;

    }



}
Y_UTEST_DONE()


