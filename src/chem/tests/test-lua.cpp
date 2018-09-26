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

    eqs.compile_for(lib);

    const size_t     M = eqs.M;
    vector<double> C(M+2);

    eqs.computeK(0);
    if(eqs.normalize(C))
    {
        lib.display(std::cerr,C);
    }
    else
    {
        std::cerr << "Unable to normalize" << std::endl;
    }

}
Y_UTEST_DONE()
