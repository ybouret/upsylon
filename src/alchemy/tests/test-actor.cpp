
#include "y/alchemy/actor.hpp"
#include "y/alchemy/library.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace alchemy;


Y_UTEST(actor)
{

    library        lib;
    const species &h = lib("H+",1);
    const species &w = lib("HO-",1);  

    actor _h(h,1);
    actor _w(w,1);

    _h.display(std::cerr,lib) << std::endl;
    _w.display(std::cerr,lib) << std::endl;

}
Y_UTEST_DONE()

