

#include "y/utest/run.hpp"
#include "y/alchemy/equilibria.hpp"
#include "y/lua++/state.hpp"

using namespace upsylon;
using namespace Alchemy;

Y_UTEST(lua)
{

    Lua::VM vm = new Lua::State();
    string  info;
    for(int i=1;i<argc;++i)
    {
        info << ' ' << argv[i];
    }
    
    Equilibria eqs;
    eqs.parse(info,*vm);
    
}
Y_UTEST_DONE()

