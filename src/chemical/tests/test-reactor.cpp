
#include "y/chemical/reactor.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Chemical;

Y_UTEST(reactor)
{
    
    Library          lib;
    Equilibria       eqs;
    Lua::VM          vm = new Lua::State();
    
    for(int i=1;i<argc;++i)
    {
        eqs(argv[i],lib,vm);
    }

    //eqs.load("dummy:-A:-2B:3C:4D:=1",lib,vm);
    
    std::cerr << "lib=" << lib << std::endl;
    std::cerr << "eqs=" << eqs << std::endl;

    //Verbosity = false;
    Reactor cs(lib,eqs,Equilibrium::Minimal);

    Vector C(cs.M,0);
    lib.drawC(C,alea);

    for(const SNode *node=cs.lib->head();node;node=node->next)
    {
        const Species &sp = ***node;
        if(1==sp.rating)
        {
            if( alea.to<double>() > 0.75 ) C[sp.indx] *= -1;
        }
    }


    lib.display(std::cerr << "C=",C) << std::endl;

    cs.balance(C);
    
    lib.display(std::cerr << "C=",C) << std::endl;

    
}
Y_UTEST_DONE()

