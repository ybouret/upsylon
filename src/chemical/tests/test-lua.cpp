#include "y/utest/run.hpp"



#include "y/chemical/system.hpp"
#include "y/chemical/lua/eqs.hpp"
#include "y/utest/run.hpp"
#include "y/mkl/tao.hpp"

using namespace upsylon;
using namespace Chemical;

Y_UTEST(lua)
{
    Library        lib;
    Lua::VM        lvm = new Lua::State;
    LuaEquilibria  eqs(lib,lvm);


    for(int i=1;i<argc;++i)
    {
        lvm->doFile(argv[i]);
    }

    const string sys = "sys";
    if(!lvm->exists(sys))
        return 0;

    eqs.append(sys);


    std::cerr << "lib=" << lib << std::endl;
    std::cerr << "eqs=" << eqs << std::endl;


    System cs(lib,eqs,Equilibrium::Utterly);


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
    }



}
Y_UTEST_DONE()

