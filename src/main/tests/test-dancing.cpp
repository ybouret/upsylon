#include "y/counting/dancing.hpp"
#include "y/utest/run.hpp"
#include "y/string/convert.hpp"

using namespace upsylon;

Y_UTEST(dancing)
{
    size_t n=7; if(argc>1) n= string_convert::to<size_t>(argv[1],"n");
    size_t k=2; if(argc>2) k= string_convert::to<size_t>(argv[2],"k");
    dancing dance(n,k);
}
Y_UTEST_DONE()

