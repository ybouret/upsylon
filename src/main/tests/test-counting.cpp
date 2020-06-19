#include "y/counting/mloop.hpp"
#include "y/counting/comb.hpp"
#include "y/counting/perm.hpp"
#include "y/counting/symm-pair.hpp"
#include "y/type/point3d.hpp"

#include "y/utest/run.hpp"

using namespace upsylon;

namespace {
 
    static inline void infoOn(counting &loop)
    {
        std::cerr << "index=" << loop.index << "/count=" << loop.count << std::endl;
        std::cerr << loop << std::endl;
        for(loop.boot();loop.good();loop.next())
        {
            std::cerr << "\t" << loop << " @" << loop.index << std::endl;
        }
    }
    
}

Y_UTEST(counting)
{
    
    {
        std::cerr << "mloop3D" << std::endl;
        const point3d<int> lo(1,2,3);
        const point3d<int> up(3,4,5);
        mloop< int,point3d<int> > loop(lo,up);
        infoOn(loop);
    }

    {
        combination comb(7,2);
        std::cerr << "comb" << std::endl;
        infoOn(comb);
    }
    
    {
        std::cerr << "perm" << std::endl;
        permutation perm(4);
        infoOn(perm);
    }
    {
        std::cerr << "symm" << std::endl;
        symm_pair symm(6);
        infoOn(symm);
    }
    
}
Y_UTEST_DONE()

