

#include "y/alchemy/equilibria.hpp"
#include "y/alchemy/library.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace alchemy;

Y_UTEST(eqs)
{
    
    library        lib;
    equilibria     eqs;
    const species &h = lib("H+",1);
    const species &w = lib("HO-",1);
    
    equilibrium &water = eqs("water", 1e-14);  water(1,h); water(1,w);
    equilibrium &toto  = eqs("toto", 1e-4); toto(2,h); toto(-1,w);
    
    std::cerr << eqs << std::endl;
    
    lib.compile();
    eqs.compile();
    std::cerr << eqs << std::endl;

    
    vector<double> C(lib->size(),0);
    lib.display(std::cerr,C) << std::endl;
    
    
    
    std::cerr << water.name << " -> " << water.compute(water.K(0),C) << std::endl;



}
Y_UTEST_DONE()
