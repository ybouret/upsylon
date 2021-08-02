

#include "y/alchemy/equilibria.hpp"
#include "y/alchemy/library.hpp"
#include "y/utest/run.hpp"
#include "y/yap.hpp"

using namespace upsylon;
using namespace Alchemy;

Y_UTEST(eqs)
{

    Library        lib;
    Equilibria     eqs;

    const Species &h  = lib("H+",1);
    const Species &w  = lib("HO-",1);
    const Species &ah = lib("AH",0);
    const Species &am = lib("Am",-1);

    Equilibrium &water = eqs("water", 1e-14);  water(1,h); water(1,w);
    Equilibrium &weak  = eqs("weak", 1e-4);    weak(1,h); weak(1,am); weak(-1,ah);

    std::cerr << lib << std::endl;
    std::cerr << eqs << std::endl;

    matrix<long> Nu_l(eqs->size(), lib->size());
    eqs.fill(Nu_l);
    std::cerr << "Nu=" << Nu_l << std::endl;


    matrix<apz> Nu_z(eqs->size(), lib->size());
    eqs.fill(Nu_z);
    std::cerr << "Nu=" << Nu_z << std::endl;

    matrix<apq> Nu_q(eqs->size(), lib->size());
    eqs.fill(Nu_q);
    std::cerr << "Nu=" << Nu_q << std::endl;
    
}
Y_UTEST_DONE()
