
#include "y/alchemy/actor.hpp"
#include "y/alchemy/library.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Alchemy;


Y_UTEST(actor)
{

    Library        lib;
    const Species &h = lib("H+",  1);
    const Species &w = lib("HO-",-1);

    Actor _h(h,2);
    Actor _w(w,1);

    std::cerr << _h << std::endl;
    std::cerr << _w << std::endl;
    


}
Y_UTEST_DONE()

