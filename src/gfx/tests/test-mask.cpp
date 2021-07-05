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

    m.free();

    for(unit_t i=-4;i<=4;++i)
    {
        for(unit_t j=-4;j<=4;++j)
        {
            m += coord(i,j);
        }
    }

    for(mask::iterator it=m.begin();it!=m.end();++it)
    {
        std::cerr << *it << std::endl;
    }
    m.get_root().graphViz("mask4.dot");
    std::cerr << "size=" << m.size() << std::endl;
    
}
Y_UTEST_DONE()

