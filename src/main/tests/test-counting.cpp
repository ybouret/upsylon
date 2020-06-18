#include "y/counting/mloop.hpp"
#include "y/counting/comb.hpp"
#include "y/counting/perm.hpp"
#include "y/type/point3d.hpp"

#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(counting)
{
    {
        const point3d<int> lo(1,2,3);
        const point3d<int> up(4,5,6);
        mloop< int,point3d<int> > loop(lo,up);
        std::cerr << "loop: index=" << loop.index << std::endl;
        std::cerr << "loop: count=" << loop.count << std::endl;

        std::cerr << "loop: from " << loop.lower << " to " << loop.upper << " : " << loop.count << std::endl;
        std::cerr << "      curr:" << *loop << " @index=" << loop.index << std::endl;
        loop.boot();
        std::cerr << "      curr:" << *loop << " @index=" << loop.index << std::endl;
    }

    {
        combination comb(6,2);
        std::cerr << "comb: index=" << comb.index <<  " : " << comb << std::endl;
        std::cerr << "comb: count=" << comb.count << std::endl;
    }
    
    {
        permutation perm(6);
        std::cerr << "perm: index=" << perm.index << " : " << perm << std::endl;
        std::cerr << "perm: count=" << perm.count << std::endl;
    }
}
Y_UTEST_DONE()

