
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

    std::cerr << "Nu  = " << cs.Nu  << std::endl;
    std::cerr << "NuT = " << cs.NuT << std::endl;

    vector<double> C(cs.M,0);
    lib.display(std::cerr << "C=",C) << std::endl;
    if(cs.isRegular(C,0.0))
    {
        std::cerr << "regular system" << std::endl;
    }
    else
    {
        std::cerr << "singular system" << std::endl;
    }
    cs.display_state();


    std::cerr << "using upgrade..." << std::endl;

    if(cs.solve(C,0.0))
    {

    }

    std::cerr << std::endl;

    std::cerr << "From alea" << std::endl;
    lib.draw(alea,C);
    if(cs.isRegular(C))
    {
        std::cerr << "regular system" << std::endl;
    }
    else
    {
        std::cerr << "singular system" << std::endl;
    }

    if( cs.solve(C) )
    {

    }
    



}
Y_UTEST_DONE()

