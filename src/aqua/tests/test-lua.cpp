#include "y/aqua/lua/interface.hpp"
#include "y/aqua/engine.hpp"
#include "y/utest/run.hpp"
#include "y/fs/vfs.hpp"
#include "y/ios/ocstream.hpp"

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

    const size_t   M = lib.entries();
    vector<double> C0(M,0);
    cs.computeK(0);
    ini.find(C0,cs);
    lib.show(std::cerr << "C0=", C0);

    Boot add("add");
    if( vm->exists(add.name) )
    {
        __Lua::Load(add,add.name,lib,vm); std::cerr << add << std::endl;
        add.init(lib,eqs);
        vector<double> Cb(M,0);
        add.find(Cb,cs);
        lib.show(std::cerr << "Cb=", Cb);

        const double   V0 = 10;
        const double   Vmax = V0+V0;
        vector<double> Cmix(M,0);

        string   saveName = vfs::get_base_name(argv[1]);
        vfs::change_extension(saveName, "dat");
        std::cerr << "save to " << saveName  << std::endl;
        ios::ocstream::overwrite(saveName);

        const size_t iproton = lib["H+"].indx;


        for(double V=0;V<=Vmax;V+=0.01)
        {
            std::cerr << ".";
            const double Vtot = V0 + V;
            for(size_t j=M;j>0;--j)
            {
                Cmix[j] = (C0[j] * V0 + Cb[j] * V)/Vtot;
            }
            if(!cs.forward(Cmix))
            {
                std::cerr << "couldn't mix@" << V << std::endl;
            }
            {
                ios::ocstream fp(saveName,true);
                fp("%g",V);
                fp(" %g", -log10( Cmix[iproton]) );
                fp << '\n';
            }
        }
        std::cerr << std::endl;

    }

    
}
Y_UTEST_DONE()


