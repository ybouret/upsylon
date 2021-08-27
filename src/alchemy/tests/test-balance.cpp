

#include "y/alchemy/reactor.hpp"
#include "y/utest/run.hpp"
#include "y/yap.hpp"
#include "y/sequence/vector.hpp"
#include "support.hpp"
#include "y/alchemy/weak-acid.hpp"
#include "y/mkl/kernel/apk.hpp"
#include "y/lua++/state.hpp"

using namespace upsylon;
using namespace Alchemy;

Y_UTEST(balance)
{

    Library        lib;
    Equilibria     eqs;
    Lua::VM        vm = new Lua::State();

    for(int i=1;i<argc;++i)
    {
        const string rx = argv[i];
        eqs(rx,lib,vm);
    }

    if(false)
    {
        eqs.parse("dummy:-2A:-B:C:3D:@1",lib,vm);
    }

    //if(false)
    {
        eqs.parse("combine:-X:-Y:@10.2",lib,vm);
    }

    // lib << "Na+" << "Cl-";

    std::cerr << "<System>" << std::endl;
    std::cerr << lib << std::endl;
    std::cerr << eqs;
    std::cerr << "<System/>" << std::endl << std::endl;


    //Verbosity = false;
    Reactor cs(lib,eqs, Equilibrium::Minimal);

    
    
    Vector  C(cs.M,0);
    lib.draw(alea,C);
    for(size_t j=cs.M;j>0;--j)
    {
        switch( lib(j).rating )
        {
            case 0: C[j] = 0; break;
            case 1:
                if(alea.to<double>()>0.7) C[j] = -C[j];
                break;
            default: C[j] = -C[j]; break;
        }
    }

    cs.balance(C);
    std::cerr << "C=" << C << std::endl;

    

}
Y_UTEST_DONE()

