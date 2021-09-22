
#include "y/chemical/system.hpp"
#include "y/chemical/lua/eqs.hpp"
#include "y/utest/run.hpp"
#include "y/mkl/tao.hpp"
#include "y/mkl/kernel/apk.hpp"

using namespace upsylon;
using namespace Chemical;
using namespace mkl;

Y_UTEST(sys)
{

    Library        lib;
    Lua::VM        lvm = new Lua::State;
    LuaEquilibria  eqs(lib,lvm);

    for(int i=1;i<argc;++i)
    {
        eqs << argv[i];
    }

    

    if(false)
    {
        lib << "A" << "B" << "C" << "D" << "E" << "F" << "G" << "H";
        eqs<<"+dum1:-2A:B:2C:3Q:=1";
        eqs<<"+dum2:-D:E:3C:Q:=2";
        eqs<<"+dum3:-F:G:4C:Q:=3";
        eqs<<"+dum4:-C:-D:3H:=4";
    }

    

    std::cerr << "lib=" << lib << std::endl;
    std::cerr << "eqs=" << eqs << std::endl;


    //Verbosity = false;
    System cs(lib,eqs,Equilibrium::Minimal);

    return 0;
    Vector C(cs.M,0);

    for(size_t iter=0;iter<1;++iter)
    {
        lib.drawC(C,alea);
        for(size_t i=cs.M;i>0;--i)
        {
            if(alea.choice()) C[i] = -C[i];
        }

        lib.display(std::cerr << "initial = ",C) << std::endl;


        if(cs.balancePrimary(C))
        {
            lib.display(std::cerr << "success = ",C) << std::endl;

            cs.showPrimary(std::cerr,C,2);
            cs.showReplica(std::cerr,C,2);


        }
        else
        {
            lib.display(std::cerr << "failure = ",C) << std::endl;
        }
        return 0;
    }

    return 0;

    std::cerr << std::endl << "Searching..." << std::endl;
    for(size_t iter=0;iter<1024;++iter)
    {
        for(size_t i=cs.N;i>0;--i)
        {
            cs.xi[i] = Library::RandomC(alea) * alea.symm<double>();
        }
        std::cerr << "xi=" << cs.xi << std::endl;
        tao::mul(C,cs.NuT,cs.xi);
        lib.display(std::cerr << "initial = ",C) << std::endl;

        if(!cs.balancePrimary(C))
        {
            throw exception("failure balancePrimary");
        }
        else
        {
            lib.display(std::cerr << "primary   = ",C) << std::endl;
        }
        break;
    }


}
Y_UTEST_DONE()
