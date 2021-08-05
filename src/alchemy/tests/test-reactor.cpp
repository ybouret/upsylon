
#include "y/alchemy/reactor.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Alchemy;


Y_UTEST(reactor)
{

    Library        lib;
    Equilibria     eqs;

    const Species &h  = lib("H+",1);
    const Species &w  = lib("HO-",1);
    const Species &ah = lib("AH",0);
    const Species &am = lib("Am",-1);
    (void) lib("Na+",1);
    (void) lib("Cl-",-1);
    
    Equilibrium &water = eqs("water", 1e-14);  water(1,h); water(1,w);
    Equilibrium &weak  = eqs("weak", 1e-4);    weak(1,h); weak(1,am); weak(-1,ah);

    std::cerr << lib << std::endl;
    std::cerr << eqs << std::endl;

    Reactor cs(lib,eqs);



    vector<double> C(cs.M,0);
    lib.draw(alea,C);
    for(size_t j=cs.M;j>0;--j)
    {
        if(cs.active[j])
        {
            if(alea.choice()) C[j] = -C[j];
        }
        else
        {
            C[j] = 0;
        }
    }
    lib.display(std::cerr << "C=",C) << std::endl;
    std::cerr << "C=" << C << std::endl;
    std::cerr << "Nu  = " << cs.Nu  << std::endl;
    std::cerr << "NuT = " << cs.NuT << std::endl;

    

}
Y_UTEST_DONE()

