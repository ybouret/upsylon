


#include "y/chemical/library.hpp"
#include "y/chemical/equilibria.hpp"

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
    Equilibrium &water = eqs("water","=10^(-14)",vm);
    Equilibrium &acid  = eqs("acid","@f",vm);

    std::cerr << eqs << std::endl;
    
#if 0
    ConstEquilibrium eq("eq",1.2);
    for(int i=1;i<argc;++i)
    {
        eq(argv[i],lib);
    }
    eq.display(std::cerr) << std::endl;
#endif
    
    
}
Y_UTEST_DONE()

