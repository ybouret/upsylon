
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
    
    Reactor cs(lib,eqs,Equilibrium::Minimal);
    
    
}
Y_UTEST_DONE()

