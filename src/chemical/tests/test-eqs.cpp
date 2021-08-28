


#include "y/chemical/library.hpp"
#include "y/chemical/equilibrium.hpp"

#include "y/utest/run.hpp"
#include "y/ios/osstream.hpp"

using namespace upsylon;
using namespace Chemical;

Y_UTEST(eqs)
{
    Library          lib;
    ConstEquilibrium eq("eq",1.2);
    for(int i=1;i<argc;++i)
    {
        eq(argv[i],lib);
    }
    eq.display(std::cerr) << std::endl;
}
Y_UTEST_DONE()

