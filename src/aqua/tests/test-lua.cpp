#include "y/aqua/lua/interface.hpp"
#include "y/aqua/engine.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Aqua;


Y_UTEST(lua)
{
    Engine cs;
    Lua::VM vm = new Lua::State();
    for(int i=1;i<argc;++i)
    {
        vm->doFile(argv[i]);
    }

    Library    lib;        __Lua::Load(lib,"lib",vm);        std::cerr << lib << std::endl;
    Equilibria eqs;        __Lua::Load(eqs,"eqs",lib,vm);    std::cerr << eqs << std::endl;
    Boot       ini("ini"); __Lua::Load(ini,ini.name,lib,vm); std::cerr << ini << std::endl;

    lib.init();
    cs.init(lib,eqs);
    ini.init(lib,eqs);

    vector<double> C(lib.entries(),0);
    cs.computeK(0);
    ini.find(C,cs);
    lib.show(std::cerr << "C=", C);

    Boot add("add");
    if( vm->exists(add.name) )
    {
        add.verbose = true;
        __Lua::Load(add,add.name,lib,vm); std::cerr << add << std::endl;
        add.init(lib,eqs);
        vector<double> Cb(lib.entries(),0);
        add.find(Cb,cs);
        lib.show(std::cerr << "Cb=", Cb);
    }

    
}
Y_UTEST_DONE()


