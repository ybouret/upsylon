
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

    //lib << "Na+" << "Cl-";

    std::cerr << "lib=" << lib << std::endl;
    std::cerr << "eqs=" << eqs << std::endl;


    //Verbosity = false;
    Reactor cs(lib,eqs,Equilibrium::Minimal);


    
    Vector C(cs.M,0);
    lib.drawC(C,alea);

    for(const SNode *node=cs.lib->head();node;node=node->next)
    {
        const Species &sp = ***node;
        if( alea.to<double>() > 0.7 ) C[sp.indx] *= -1;

    }


    lib.display(std::cerr << "C=",C) << std::endl;

    if( cs.balance(C) )
    {
        std::cerr << "SUCCESS" << std::endl;
    }
    else
    {
        std::cerr << "FAILURE" << std::endl;
    }
    
    lib.display(std::cerr << "C=",C) << std::endl;

    
}
Y_UTEST_DONE()

