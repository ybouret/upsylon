#include "y/chem/lua/io.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Chemical;

Y_UTEST(lua)
{

    Lua::VM vm = new Lua::State();
    for(int i=1;i<argc;++i)
    {
        vm->doFile(argv[i]);
    }
    vm->doString("print(\"species \",species)");
    vm->doString("print(\"eqs     \",species)");
    
    Library lib;
    __luaIO::add(lib,vm,"species" );
    std::cerr << "lib=#" << lib.size() << std::endl;
    std::cerr << lib << std::endl;



    Equilibria eqs;
    __luaIO::add(eqs,vm,"eqs",lib);
    std::cerr << "eqs=#" << eqs.size() << std::endl;
    std::cerr << eqs << std::endl;

}
Y_UTEST_DONE()
