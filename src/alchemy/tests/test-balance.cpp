

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
    Lua::State     vm;
    
    //if(false)
    {
        (void) eqs.water(lib,1e-14);
    }

    //if(false)
    {
        eqs.parse("acetic:H+:A-:-AH:@10^(-4.8)",lib,vm);
        //(void) WeakAcid::Add(eqs,lib,"acetic",  "AH",  "A-",  pow(10.0,-4.8));
    }

    if(false)
    {
        (void) WeakAcid::Add(eqs,lib,"ammonia","NH4+", "NH3", pow(10.0,-9.2));
    }

    if(false)
    {
        (void) WeakAcid::Add(eqs,lib,"Ox1","OxH2", "OxH-",    pow(10.0,-1.2));
        (void) WeakAcid::Add(eqs,lib,"Ox2","OxH-", "OxH--",   pow(10.0,-4.3));
    }

    if(false)
    {
        (void) WeakAcid::Add(eqs,lib,"Citric1","CitH3",  "CitH2-", pow(10.0,-3.13));
        (void) WeakAcid::Add(eqs,lib,"Citric2","CitH2-", "CitH--", pow(10.0,-4.76));
        (void) WeakAcid::Add(eqs,lib,"Citric3","CitH--", "Cit---", pow(10.0,-6.40));

    }


    if(false)
    {
        eqs.parse("dummy:-2A:-B:C:3D:@1",lib,vm);
    }

    if(false)
    {
        eqs.parse("combine:-X:-Y:@10.2",lib,vm);
    }

    lib << "Na+" << "Cl-";

    std::cerr << lib << std::endl;
    std::cerr << eqs << std::endl;

    Reactor cs(lib,eqs, Equilibrium::Minimal);



    
    Vector  C(cs.M,0);
    lib.draw(alea,C);
    for(size_t j=cs.M;j>0;--j)
    {
        switch( lib(j).rating )
        {
            case 0: C[j] = 0; break;
            case 1:
                //C[j] = 0;
                break;
            default: C[j] = -C[j]; break;
        }
    }

    cs.balance(C);
    std::cerr << "C=" << C << std::endl;

    

}
Y_UTEST_DONE()

