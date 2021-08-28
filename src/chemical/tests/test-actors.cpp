


#include "y/chemical/actors.hpp"
#include "y/chemical/library.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Chemical;

Y_UTEST(actors)
{
    Library lib;
    
    const Species &h = lib("H+");
    const Species &w = lib("HO-");
    
    std::cerr << lib << std::endl;
    
    {
        Actor a(1,h);
        Actor b(2,w);
        std::cerr << a << ", " << b << std::endl;
    }
    
    Actors A;
    A(1,h);
    A(2,w);
    
    std::cerr << A << std::endl;
    
}
Y_UTEST_DONE()

