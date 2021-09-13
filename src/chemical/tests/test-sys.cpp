
#include "y/chemical/system.hpp"
#include "y/utest/run.hpp"
#include "y/mkl/tao.hpp"

using namespace upsylon;
using namespace Chemical;
using namespace mkl;

Y_UTEST(sys)
{

    Library          lib;
    Equilibria       eqs;
    Lua::VM          vm = new Lua::State();

    for(int i=1;i<argc;++i)
    {
        eqs(argv[i],lib,vm);
    }

    // eqs.load("dummy:-A:-2B:3C:4D:=1",lib,vm);


    //if(false)
    {
        lib << "A" << "B" << "C" << "D" << "E" << "F" << "G" << "H";
        eqs.load("dum1:-A:B:2C:Q:=1",lib,vm);
        eqs.load("dum2:-D:E:3C:Q:=2",lib,vm);
        eqs.load("dum3:-F:G:4C:Q:=3",lib,vm);
        eqs.load("dum4:-C:H:=4",lib,vm);
    }
    
    lib << "Na+" << "Cl-";

    std::cerr << "lib=" << lib << std::endl;
    std::cerr << "eqs=" << eqs << std::endl;


    //Verbosity = false;
    System cs(lib,eqs,Equilibrium::Minimal);

    return 0;
    
    Vector C(cs.M,0);

    for(size_t iter=0;iter<0;++iter)
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

        }
        else
        {
            lib.display(std::cerr << "failure = ",C) << std::endl;
        }
        return 0;
    }


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
