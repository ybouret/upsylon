
#include "y/chemical/reactor.hpp"
#include "y/chemical/lua/eqs.hpp"
#include "y/utest/run.hpp"
#include "y/mkl/tao.hpp"

using namespace upsylon;
using namespace Chemical;

Y_UTEST(solve)
{
    Library        lib;
    Lua::VM        lvm = new Lua::State;
    LuaEquilibria  eqs(lib,lvm);
    
    for(int i=1;i<argc;++i)
    {
        eqs << argv[i];
    }

    std::cerr << lib << std::endl;
    std::cerr << eqs << std::endl;

    Reactor cs(lib,eqs,Equilibrium::Utterly);
    Vector  C(cs.M,0);

    lib.drawC(C,alea);
    lib.display(std::cerr,C) << std::endl;


    cs.loadK(0.0);
    cs.computeGamma(C);
    std::cerr << "K="     << cs.K << std::endl;
    std::cerr << "Gamma=" << cs.Gamma << std::endl;
    cs.computeGammaAndJ(C);
    std::cerr << "Gamma=" << cs.Gamma << std::endl;
    std::cerr << "J=" << cs.J << std::endl;

    if(!cs.solve(C))
    {
        std::cerr << "singular system" << std::endl;
    }

    Vector rate(cs.M,0);
    lib.drawC(rate,alea);

    std::cerr << "rate = " << rate << std::endl;
    cs.damp(rate,C);
    std::cerr << "damp = " << rate << std::endl;

    mkl::tao::mul(cs.xi,cs.J,rate);
    std::cerr << "residual=" << cs.xi << std::endl;
    
    
}
Y_UTEST_DONE()

