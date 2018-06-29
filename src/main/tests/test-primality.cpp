#include "y/code/primality.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(primality)
{
    size_t n = 100;
    if(argc>1)
    {
        n = atol(argv[1]);
    }
    size_t p=0;
    size_t i=0;
    do
    {
        p = primality::next(p);
        std::cerr << ' ' << p;
        ++i;
        if(0==(i&15)) std::cerr << std::endl;
        ++p;
    } while(i<n);
    std::cerr << std::endl;
}
Y_UTEST_DONE()

