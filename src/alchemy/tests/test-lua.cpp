

#include "y/utest/run.hpp"
#include "y/alchemy/equilibria.hpp"
#include "y/alchemy/library.hpp"

#include "y/lua++/state.hpp"

using namespace upsylon;
using namespace Alchemy;

Y_UTEST(lua)
{
    Library lib;
    Equilibria eqs;

    Lua::VM vm = new Lua::State();
    for(int i=1;i<argc;++i)
    {
        const string info = argv[i];
        eqs.parse(info,lib,*vm);
    }
    
    std::cerr << lib << std::endl;
    std::cerr << eqs << std::endl;
}
Y_UTEST_DONE()

