

#include "y/alchemy/reactor.hpp"
#include "y/utest/run.hpp"
#include "y/yap.hpp"
#include "y/sequence/vector.hpp"
#include "support.hpp"
#include "y/alchemy/weak-acid.hpp"


using namespace upsylon;
using namespace Alchemy;

Y_UTEST(balance)
{

    Library        lib;
    Equilibria     eqs;

    (void) eqs.water(lib,1e-14);

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
        dummy(-1,lib.get("A"));
        dummy(-2,lib.get("B"));
        dummy(1,lib.get("C"));
        dummy(3,lib.get("D"));
    }

    //if(false)
    {
        Equilibrium &combine = eqs("combine",10.2);
        combine(-1, lib.get("X"));
        combine(-1, lib.get("Y"));
    }


    //lib.get("Na+");
    //lib.get("Cl-");

    std::cerr << lib << std::endl;
    std::cerr << eqs << std::endl;

    Reactor cs(lib,eqs);

    Vector  C(cs.M,0);
    lib.draw(alea,C);
    for(size_t j=cs.M;j>0;--j)
    {
        if( alea.choice() ) C[j] = -C[j];
        //if( !cs.active[j]) C[j] = 0;
    }

    //lib(C,"H+") = - alea.to<double>();
    //lib(C,"H+") *= -1;
    cs.balance(C);

    

}
Y_UTEST_DONE()

