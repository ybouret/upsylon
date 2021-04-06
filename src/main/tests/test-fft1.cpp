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

        for(size_t n=1;n<=4096; n <<= 1)
        {
            vector<cplx> source(n,0);
            vector<cplx> fourier(n,0);
            support::fill1D(source);

            for(size_t i=n;i>0;--i) fourier[i] = source[i];

            fft1::forward(&fourier[1].re-1,n);
            fft1::reverse(&fourier[1].re-1,n);

            T rms = 0;
            for(size_t i=n;i>0;--i)
            {
                const cplx delta = fourier[i]/n-source[i];
                rms += delta.mod2();
            }
            rms /= n;
            rms = mkl::sqrt_of(rms);
            std::cerr << "rms#" << n << " = " << rms << std::endl;


        }
    }
}

Y_UTEST(fft1)
{
    do_test<float>();
    do_test<double>();

}
Y_UTEST_DONE()

