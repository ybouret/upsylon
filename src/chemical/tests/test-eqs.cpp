


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
    
    for(int i=1;i<argc;++i)
    {
        (void) eqs(argv[i],lib,vm);
    }
    
    std::cerr << eqs << std::endl;
    
}
Y_UTEST_DONE()

