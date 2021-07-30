
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

    actor _h(h,2);
    actor _w(w,1);

    std::cerr << "before LIB compile" << std::endl;
    std::cerr << _h << " something" << std::endl;
    std::cerr << _w << " something" << std::endl;
    
    lib.compile();
    std::cerr << "After LIB compile" << std::endl;
    std::cerr << _h << " something" << std::endl;
    std::cerr << _w << " something" << std::endl;
    
}
Y_UTEST_DONE()

