


#include "y/chemical/library.hpp"
#include "y/chemical/eqs/db.hpp"
#include "y/chemical/leading.hpp"

#include "y/utest/run.hpp"
#include "y/ios/osstream.hpp"

using namespace upsylon;
using namespace Chemical;

Y_UTEST(eqs)
{ 
    Library          lib;
    Equilibria       eqs;
    Lua::VM          vm = new Lua::State();
    vm->doString("function f(t) return 10^(-4.18) end");
    
    for(int i=1;i<argc;++i)
    {
        eqs(argv[i],lib,vm);
    }
    
    std::cerr << lib << std::endl;
    std::cerr << eqs << std::endl;

    iMatrix NuT;
    for(const ENode *node=eqs->head();node;node=node->next)
    {
        const Equilibrium  &eq = ***node;
        const Leading       leading(eq,NuT);
        std::cerr << leading << std::endl;
    }

    Vector C(lib->size(),1.2);
    lib.drawC(C,alea);
    lib.display(std::cerr,C) << std::endl;
}
Y_UTEST_DONE()

