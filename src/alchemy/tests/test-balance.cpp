

#include "y/alchemy/reactor.hpp"
#include "y/utest/run.hpp"
#include "y/yap.hpp"
#include "y/sequence/vector.hpp"
#include "support.hpp"

using namespace upsylon;
using namespace Alchemy;

Y_UTEST(balance)
{

    Library        lib;
    Equilibria     eqs;

    const Species &h  = lib("H+",1);
    const Species &w  = lib("HO-",1);
    const Species &ah = lib("AH",0);
    const Species &am = lib("Am",-1);

    (void) lib("Na+",1);
    (void) lib("Cl-",-1);

    Equilibrium &water    = eqs("water",  1e-14); water(1,h); water(1,w);
    Equilibrium &acetic   = eqs("acetic", 1e-4);  acetic(1,h); acetic(1,am); acetic(-1,ah);

    if(false)
    {
        const Species &nh4     = lib("NH4+",1);
        const Species &nh3     = lib("NH3",0);
        Equilibrium   &ammonia = eqs("ammonia", 1e-9); ammonia(1,h); ammonia(1,nh3); ammonia(-1,nh4);
    }

    std::cerr << lib << std::endl;
    std::cerr << eqs << std::endl;

    Reactor cs(lib,eqs);
    std::cerr << "Nu=" << cs.Nu << std::endl;

    cs.complete();
    
    return 0;

    Vector C(cs.M,0);
    lib.draw(alea,C);
    for(size_t j=cs.M;j>0;--j)
    {
        if(cs.active[j])
        {

        }
        else C[j]=0;
    }

    lib(C,"H+") *= -1;

    lib.display(std::cerr << "C=",C) << std::endl;
    std::cerr << "C   = " << C << std::endl;
    std::cerr << "Nu  = " << cs.Nu  << std::endl;
    std::cerr << "NuT = " << cs.NuT << std::endl;
    cs.balance(C);

    

}
Y_UTEST_DONE()

