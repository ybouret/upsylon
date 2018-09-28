#include "y/chem/lua/io.hpp"
#include "y/utest/run.hpp"
#include "y/ios/ocstream.hpp"

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
    __luaIO::add(lib,vm,"species" );
    std::cerr << "lib=#" << lib.size() << std::endl;
    std::cerr << lib << std::endl;

    Equilibria eqs;
    __luaIO::add(eqs,vm,"eqs",lib);
    std::cerr << "eqs=#" << eqs.size() << std::endl;
    std::cerr << eqs << std::endl;

    eqs.compile_for(lib);

    const size_t     M = eqs.M;
    vector<double>   C0(M+2);

    eqs.computeK(0);

    if(eqs.normalize(C0))
    {
        lib.display(std::cerr,C0);
    }
    else
    {
        std::cerr << "Unable to normalize" << std::endl;
    }

    {
        std::cerr << "Initial Solution" << std::endl;
        Boot::Loader ld;
        __luaIO::add(ld,vm,"ini",lib);
        std::cerr << ld << std::endl;

        std::cerr << "Booting..." << std::endl;
        if( eqs.boot(C0,ld) )
        {
            lib.display(std::cerr,C0);
        }
        else
        {
            std::cerr << "Unable to boot" << std::endl;
        }
    }


    if( vm->exists("inject") )
    {
        std::cerr << "Injected Solution" << std::endl;
        Boot::Loader ld;
        __luaIO::add(ld,vm,"inject",lib);
        std::cerr << ld << std::endl;
        vector<double> C1(M+3);
        if( eqs.boot(C1,ld) )
        {
            lib.display(std::cerr,C1);
            vector<double> C(M);

            const double V0 = 10;
            const double dV = V0/100.0;
            ios::ocstream fp("output.dat");
            fp("#V1 pH"); lib.header(fp) << "\n";
            for(double V1=0;V1<=2*V0;V1+=dV)
            {
                const double Vtot = V1 + V0;
                eqs.mix(C, C0, V0/Vtot, C1, V1/Vtot);
                fp("%.15g %.15g",V1, lib.pH(C) ); lib.xprint(fp,C) << "\n";
            }


        }
        else
        {
            std::cerr << "Unable to boot" << std::endl;
            return 0;
        }
    }
    
}
Y_UTEST_DONE()
