#include "y/memory/groove.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(groove)
{
    {
        memory::groove g0;
        memory::groove g1(1);
        memory::groove g2(2);
        memory::groove g3(3);
        memory::groove g4(4);
    }

    memory::groove g(0);
    for(size_t iter=0;iter<1000;++iter)
    {
        g.acquire( alea.leq(32) );
    }

}
Y_UTEST_DONE()

