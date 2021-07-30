

#include "y/alchemy/actors.hpp"
#include "y/alchemy/library.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace alchemy;

Y_UTEST(actors)
{

    library        lib;
    const species &h = lib("H+",1);
    const species &w = lib("HO-",1);

    actors A;
    A(h,10);
    A(w,1);

    A.display(std::cerr,lib.max_name) << std::endl;
    
}
Y_UTEST_DONE()

