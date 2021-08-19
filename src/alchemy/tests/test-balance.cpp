

#include "y/alchemy/reactor.hpp"
#include "y/utest/run.hpp"
#include "y/yap.hpp"
#include "y/sequence/vector.hpp"
#include "support.hpp"
#include "y/alchemy/weak-acid.hpp"
#include "y/mkl/kernel/apk.hpp"

using namespace upsylon;
using namespace Alchemy;

Y_UTEST(balance)
{

    Library        lib;
    Equilibria     eqs;

    if(false)
    {
        (void) eqs.water(lib,1e-14);
    }

    if(false)
    {
        (void) WeakAcid::Add(eqs,lib,"acetic",  "AH",  "A-",  pow(10.0,-4.8));
    }

    if(false)
    {
        (void) WeakAcid::Add(eqs,lib,"ammonia","NH4+", "NH3", pow(10.0,-9.2));
    }

    //if(false)
    {
        (void) WeakAcid::Add(eqs,lib,"Ox1","OxH2", "OxH-",    pow(10.0,-1.2));
        (void) WeakAcid::Add(eqs,lib,"Ox2","OxH-", "OxH--",   pow(10.0,-4.3));
    }

    if(false)
    {
        Equilibrium &dummy = eqs("dummy",1);
        dummy(-2,lib.get("B"));
        dummy(-1,lib.get("A"));
        dummy( 1,lib.get("C"));
        dummy( 3,lib.get("D"));
    }

    if(false)
    {
        Equilibrium &combine = eqs("combine",10.2);
        combine(-1, lib.get("X"));
        combine(-1, lib.get("Y"));
    }


    //lib.get("Na+");
    //lib.get("Cl-");

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
            case 1: break;
            default: C[j] = -C[j]; break;
        }
    }

    cs.balance(C);
    std::cerr << "C=" << C << std::endl;

    

}
Y_UTEST_DONE()

