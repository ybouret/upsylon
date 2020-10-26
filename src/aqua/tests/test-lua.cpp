#include "y/aqua/lua/interface.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Aqua;


Y_UTEST(lua)
{
    Lua::VM vm = new Lua::State();
    for(int i=1;i<argc;++i)
    {
        vm->doFile(argv[i]);
    }

    Library    lib;        __Lua::Load(lib,"lib",vm);        std::cerr << lib << std::endl;
    Equilibria eqs;        __Lua::Load(eqs,"eqs",lib,vm);    std::cerr << eqs << std::endl;
    Boot       ini("ini"); __Lua::Load(ini,ini.name,lib,vm); std::cerr << ini << std::endl;

    
}
Y_UTEST_DONE()


