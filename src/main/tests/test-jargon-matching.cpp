#include "y/jargon/pattern/matching.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Jargon;

Y_UTEST(jargon_matching)
{
    Cache cache;
    if(argc>1)
    {
        Matching match(cache,argv[1]);
        
    }
}
Y_UTEST_DONE()

