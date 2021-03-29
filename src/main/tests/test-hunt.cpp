#include "y/code/hunt.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"

using namespace upsylon;


namespace
{
    template <typename T> static inline
    void test_hunt()
    {
        const size_t n = 2 + alea.lt(100);
        vector<T>    x(n,0);
        for(size_t i=2;i<=n;++i)
        {
            x[i] = x[i-1]+ alea.to<T>();
        }
        const T xmax = x[n];
        std::cerr << x[1] << " -> " << x[n] << std::endl;
        const T ampli = T(0.6) * xmax;
        const T xhalf = xmax/2;

        for(size_t iter=100;iter>0;--iter)
        {
            size_t j=alea.leq(n+10);

            hunt(xhalf+ampli*alea.symm<T>(),&x[1], n, j);
        }

    }
}


Y_UTEST(hunt)
{
    
}
Y_UTEST_DONE()

