

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
    const Species &w  = lib("HO-",-1);


    (void) lib("Na+",1);
    (void) lib("Cl-",-1);

    Equilibrium &water    = eqs("water",  1e-14); water(1,h); water(1,w);

    if(false)
    {
        const Species &ah     = lib("AH",0);
        const Species &am     = lib("A-",-1);
        Equilibrium   &acetic = eqs("acetic", 1e-4);  acetic(1,h); acetic(1,am); acetic(-1,ah);
    }

    if(false)
    {
        const Species &nh4     = lib("NH4+",1);
        const Species &nh3     = lib("NH3",0);
        Equilibrium   &ammonia = eqs("ammonia", 1e-9); ammonia(1,h); ammonia(1,nh3); ammonia(-1,nh4);
    }

    if(true)
    {
        const Species &OxH2 = lib("OxH2",0);
        const Species &OxHm = lib("OxH-",-1);
        const Species &Oxmm = lib("Oxmm",-2);
        Equilibrium   &Ox1  = eqs("Ox1",pow(10.0,-1.2)); Ox1(1,h); Ox1(1,OxHm); Ox1(-1,OxH2);
        Equilibrium   &Ox2  = eqs("Ox2",pow(10.0,-4.3)); Ox2(1,h); Ox2(1,Oxmm); Ox2(-1,OxHm);

    }

    std::cerr << lib << std::endl;
    std::cerr << eqs << std::endl;

    Reactor cs(lib,eqs);

    
    Vector  C(cs.M,0);
    lib.draw(alea,C);
    for(size_t j=cs.M;j>0;--j)
    {
        if(cs.active[j])
        {
            if( alea.choice() ) C[j] = -C[j];
        }
        else
        {
            //C[j]=0;
        }
    }

    //lib(C,"H+") = - alea.to<double>();
    //lib(C,"H+") *= -1;
    lib.display(std::cerr << "C=",C) << std::endl;
    cs.balance(C);

    

}
Y_UTEST_DONE()

