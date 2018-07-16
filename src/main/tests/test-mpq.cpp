#include "y/mpl/rational.hpp"
#include "y/utest/run.hpp"
#include "y/code/arith.hpp"

using namespace upsylon;

#define ITER (1<<10)

Y_UTEST(mpq)
{
    std::cerr << "-- mpq setup" << std::endl;
    for(size_t iter=0;iter<ITER;++iter)
    {
        const int64_t  n = alea.partial<int64_t>( alea.leq(62) ) * alea.pm();
        uint64_t d = 1+alea.partial<uint64_t>( alea.leq(62) );
        uint64_t a = abs_of(n);
        const mpq Q(n,d);
        arithmetic::simplify(a,d);
        Y_ASSERT(Q.num.n==a);
        Y_ASSERT(Q.den==d);
    }
}
Y_UTEST_DONE()

