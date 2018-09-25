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
    Library lib;
    __luaIO::add(lib, vm, "species" );
    std::cerr << "lib=#" << lib.size() << std::endl;
    std::cerr << lib << std::endl;
    
}
Y_UTEST_DONE()
