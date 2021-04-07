#include "y/fft/fft1.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include "y/sequence/vector.hpp"
#include "y/type/spec.hpp"
using namespace upsylon;

namespace
{
    template <typename T>
    static inline void do_test()
    {
        std::cerr << std::endl;
        std::cerr << "generic test<" << type_name_of<T>() << ">" << std::endl;
        typedef complex<T> cplx;

        for(size_t n=1;n<=32768; n <<= 1)
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
        
        fft1::forward<T,n>(&fourier[1].re-1);
        fft1::reverse<T,n>(&fourier[1].re-1);
        
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
        std::cerr << std::endl;
        std::cerr << "specific test<" << type_name_of<T>() << ">" << std::endl;
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
        do_test2<T,32768>();

    }

    template <typename T>
    static inline void do_test_opt()
    {
        std::cerr << std::endl;
        std::cerr << "dispatched test<" << type_name_of<T>() << ">" << std::endl;
        typedef complex<T> cplx;

        for(size_t n=1;n<=32768; n <<= 1)
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

    for(size_t i=0;i<fft1::sin_table_size;++i)
    {
        std::cerr << fft1::positive_sinus[i] << "," << fft1::negative_sinus[i] << std::endl;
    }
    do_test<float>();
    do_test<double>();
    
    do_tests<float>();
    do_tests<double>();

    do_test_opt<float>();
    do_test_opt<double>();
    
}
Y_UTEST_DONE()

