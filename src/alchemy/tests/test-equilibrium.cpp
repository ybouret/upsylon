
#include "y/alchemy/equilibrium.hpp"
#include "y/alchemy/library.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace alchemy;

Y_UTEST(eq)
{
    
    library        lib;
    const species &h = lib("H+",1);
    const species &w = lib("HO-",1);

    equilibrium::pointer water = new constant_equilibrium("water",1e-14);
    
    (*water)(1,h);
    (*water)(1,w);
    std::cerr << water->format(10,lib.max_name,0) << " (" << water->K(0) << ")" << std::endl;
    //water->display(std::cerr,lib,0.0) << std::endl;
    
    equilibrium::pointer toto = new constant_equilibrium("toto",1e-1);
    (*toto)(2,h);
    (*toto)(-1,w);
    std::cerr << toto->format(10,lib.max_name,0) << " (" << toto->K(0) << ")" << std::endl;

    
}
Y_UTEST_DONE()

