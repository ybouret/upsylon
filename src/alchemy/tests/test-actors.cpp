

#include "y/alchemy/actors.hpp"
#include "y/alchemy/library.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Alchemy;

Y_UTEST(actors)
{
    Library        lib;
    const Species &h = lib("H+",1);
    const Species &w = lib("HO-",-1);
    
    Actors A;
    A(h,10);
    A(w,1);
    
    Actors B;
    B(w,3);
    B(h,12);
    
    std::cerr << A << std::endl;
    std::cerr << B << std::endl;

}
Y_UTEST_DONE()

