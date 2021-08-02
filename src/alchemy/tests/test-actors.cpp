

#include "y/alchemy/actors.hpp"
#include "y/alchemy/library.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Alchemy;

Y_UTEST(actors)
{
#if 0
    library        lib;
    const species &h = lib("H+",1);
    const species &w = lib("HO-",-1);
    
    actors A;
    A(h,10);
    A(w,1);
    
    actors B;
    B(w,3);
    B(h,12);
    
    std::cerr << A << "|" << std::endl;
    std::cerr << B << "|" <<std::endl;

    lib.compile();
    std::cerr << A << "|" <<std::endl;
    std::cerr << B << "|" <<std::endl;
    
    A.compile();
    B.compile();
    std::cerr << A << "|" <<std::endl;
    std::cerr << B << "|" <<std::endl;
#endif
}
Y_UTEST_DONE()

