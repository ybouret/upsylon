
#include "y/utest/run.hpp"
#include "y/chemical/system.hpp"
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

    
    System cs(lib,eqs,Equilibrium::Utterly);
    
    
    
    
    
}
Y_UTEST_DONE()

