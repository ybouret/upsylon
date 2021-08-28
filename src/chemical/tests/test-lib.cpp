

#include "y/chemical/library.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Chemical;

Y_UTEST(lib)
{
    Library lib;
    
    lib("H+",1);
    lib("HO-",-1);

    std::cerr << *lib << std::endl;
    
    
}
Y_UTEST_DONE()

