

#include "y/alchemy/equilibria.hpp"
#include "y/alchemy/library.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Alchemy;

Y_UTEST(eqs)
{
#if 0
    library        lib;
    equilibria     eqs;
    const species &h  = lib("H+",1);
    const species &w  = lib("HO-",1);
    const species &ah = lib("AH",0);
    const species &am = lib("Am",-1);
    
    equilibrium &water = eqs("water", 1e-14);  water(1,h); water(1,w);
    equilibrium &weak  = eqs("weak", 1e-4);    weak(1,h); weak(1,am); weak(-1,ah);
    
    std::cerr << eqs << std::endl;
    
    lib.compile();
    eqs.compile();
    std::cerr << eqs << std::endl;

    
    vector<double> C(lib->size(),0), Ctry(C);
    for(size_t i=C.size();i>0;--i)
    {
        C[i] = alea.to<double>();
    }
    lib.display(std::cerr,C) << std::endl;
    
    std::cerr << water.name << " extents : " << water.find_extents(C) << std::endl;
    std::cerr << water.name << " -> " << water.compute(water.K(0),C) << std::endl;

    std::cerr << weak.name << " extents : " << weak.find_extents(C) << std::endl;
    std::cerr << weak.name << " -> " << weak.compute(weak.K(0),C) << std::endl;

    water.solve(C,water.K(0),Ctry);
    weak.solve(C,weak.K(0),Ctry);
#endif
    
}
Y_UTEST_DONE()
