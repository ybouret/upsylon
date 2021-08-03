
#include "y/alchemy/reactor.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Alchemy;

namespace
{

    static inline
    void drawC(addressable<double> &C)
    {
        for(size_t i=C.size();i>0;--i)
        {
            const double v = alea.to<double>();
            const double p = alea.range<double>(-14,0);
            C[i] = v * pow(10.0,p);
        }
    }
}

Y_UTEST(reactor)
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

    Reactor cs(lib,eqs);

    std::cerr << "Nu  = " << cs.Nu  << std::endl;
    std::cerr << "NuT = " << cs.NuT << std::endl;

    vector<double> C(cs.M,0);
    drawC(C);
    lib.display(std::cerr << "C=",C) << std::endl;
    cs.initialize(C,0.0);
    

}
Y_UTEST_DONE()

