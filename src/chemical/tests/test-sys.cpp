
#include "y/chemical/system.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Chemical;

Y_UTEST(sys)
{

    Library          lib;
    Equilibria       eqs;
    Lua::VM          vm = new Lua::State();

    for(int i=1;i<argc;++i)
    {
        eqs(argv[i],lib,vm);
    }

    //    eqs.load("dummy:-A:-2B:3C:4D:=1",lib,vm);

    //lib << "Na+" << "Cl-";

    std::cerr << "lib=" << lib << std::endl;
    std::cerr << "eqs=" << eqs << std::endl;


    Verbosity = false;
    System cs(lib,eqs,Equilibrium::Minimal);

    Vector C(cs.M,0);

    for(size_t iter=0;iter<16;++iter)
    {
        lib.drawC(C,alea);
        for(size_t i=cs.M;i>0;--i)
        {
            if(alea.choice()) C[i] = -C[i];
        }

        lib.display(std::cerr << "initial = ",C) << std::endl;


        if(cs.balance(C))
        {
            lib.display(std::cerr << "success = ",C) << std::endl;
        }
        else
        {
            lib.display(std::cerr << "failure = ",C) << std::endl;
        }
    }

}
Y_UTEST_DONE()
