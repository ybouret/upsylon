#include "y/mpl/pfq.hpp"
#include "y/utest/run.hpp"
#include "y/string/convert.hpp"

using namespace upsylon;

Y_UTEST(pfq)
{
    size_t n = 0;
    size_t d = 1;

    if( argc > 1 )
    {
        n = string_convert::to<size_t>(argv[1],"n");
    }

    if( argc > 2 )
    {
        d = string_convert::to<size_t>(argv[2],"d");
    }

    mpl::pfq Q(n,d);
    std::cerr << "Q=" << Q << std::endl;
    
}
Y_UTEST_DONE()

