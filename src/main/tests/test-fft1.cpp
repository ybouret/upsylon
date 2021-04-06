#include "y/fft/fft1.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include "y/sequence/vector.hpp"

using namespace upsylon;

namespace
{
    template <typename T>
    static inline void do_test()
    {
        typedef complex<T> cplx;

        for(size_t n=1;n<=256; n <<= 1)
        {
            vector<cplx> c(n,0);
            support::fill1D(c);

            fft1::forward(&c[1].re,n);
        }
    }
}

Y_UTEST(fft1)
{
    do_test<float>();
}
Y_UTEST_DONE()

