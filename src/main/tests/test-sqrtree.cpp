
#include "y/core/isqrt.hpp"
#include "y/utest/run.hpp"

#include <cstdio>
using namespace upsylon;

namespace {
    //! integer square root, raw method
    template <typename T> static inline
    T __isqrt(const T n) throw()
    {
        if(n<=1)
        {
            std::cerr << ' ' << n << std::endl;
            return n;
        }
        else
        {
            T x0 = n;
            T x1 = (n>>1);
            std::cerr << ' ' << x0;
            while(true)
            {
                x0 = x1;
                x1 = ((x0+n/x0)>>1);
                std::cerr << ' ' << x0;
                if(x1>=x0)
                {
                    std::cerr << std::endl;
                    return x0;
                }
            }
        }
    }
}


Y_UTEST(sqrtree)
{

    for(size_t i=1;i<=100;++i)
    {
        (void)__isqrt(i);
    }

}
Y_UTEST_DONE()

