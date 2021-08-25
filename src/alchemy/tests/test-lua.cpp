

#include "y/utest/run.hpp"
#include "y/alchemy/equilibria.hpp"
#include "y/alchemy/library.hpp"


using namespace upsylon;
using namespace Alchemy;

Y_UTEST(lua)
{
    Library    lib;
    Equilibria eqs;
    Lua::VM    vm = new Lua::State();

    if(argc>1)
    {
        vm->doFile(argv[1]);
        if(vm->exists("lib"))
        {
            lib.load("lib",vm);
        }

        if(vm->exists("eqs"))
        {
            eqs.load("eqs",lib,vm);
        }
    }

    std::cerr << "lib=" << std::endl << lib << std::endl;
    std::cerr << "eqs=" << std::endl << eqs << std::endl;


}
Y_UTEST_DONE()

