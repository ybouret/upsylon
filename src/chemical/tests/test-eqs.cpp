


#include "y/chemical/library.hpp"
#include "y/chemical/equilibrium.hpp"

#include "y/utest/run.hpp"
#include "y/ios/osstream.hpp"

using namespace upsylon;
using namespace Chemical;

Y_UTEST(eqs)
{
    Library lib;
    
    const Species &h = lib("H+");
    const Species &w = lib("HO-");
    
    ConstEquilibrium water("water",1e-14);
    water(1,h);
    water(1,w);
    water.display(std::cerr) << std::endl;
    
    ConstEquilibrium dummy("dummy",1);
    dummy(1,lib("Al+++"));
    dummy(3,w);
    dummy(-1,lib("AlOH3"));
    
    dummy.display(std::cerr) << std::endl;
    
    std::cerr << lib << std::endl;
}
Y_UTEST_DONE()

