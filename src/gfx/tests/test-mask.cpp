#include "y/gfx/mask.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace graphic;

Y_UTEST(mask)
{
    mask m;


    m.insert( coord(0,0) );
    m.insert( coord(1,0) );
    m.insert( coord(0,1) );
    m.insert( coord(1,1) );
    m.get_root().graphViz("mask.dot");
    for(mask::iterator it=m.begin();it!=m.end();++it)
    {
        std::cerr << *it << std::endl;
    }

}
Y_UTEST_DONE()

