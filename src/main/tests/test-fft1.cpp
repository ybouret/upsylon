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

            fft1::forward_(&fourier[1].re-1,n);
            fft1::reverse_(&fourier[1].re-1,n);

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
    
    template <typename T, const size_t n>
    static inline void do_test2()
    {
        typedef complex<T> cplx;

        vector<cplx> source(n,0);
        vector<cplx> fourier(n,0);
        support::fill1D(source);
        
        for(size_t i=n;i>0;--i) fourier[i] = source[i];
        
        fft1::forward_<T,n>(&fourier[1].re-1);
        fft1::reverse_<T,n>(&fourier[1].re-1);
        
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
    
    
    template <typename T>
    static inline void do_tests()
    {
        do_test2<T,1>();
        do_test2<T,2>();
        do_test2<T,4>();
        do_test2<T,8>();
        do_test2<T,16>();
        do_test2<T,32>();
        do_test2<T,64>();
        do_test2<T,128>();
        do_test2<T,256>();
        do_test2<T,512>();
        do_test2<T,1024>();
        do_test2<T,2048>();
        do_test2<T,4096>();
        do_test2<T,8192>();
        do_test2<T,16384>();

    }
    
}

Y_UTEST(fft1)
{
    do_test<float>();
    do_test<double>();
    
    do_tests<float>();
    do_tests<double>();
    
}
Y_UTEST_DONE()

