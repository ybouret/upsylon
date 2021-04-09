#include "y/fft/fft1.hpp"
#include "y/utest/run.hpp"
#include "y/utest/timings.hpp"
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
        std::cerr << "Generic test<" << type_name_of<T>() << ">" << std::endl;
        typedef complex<T> cplx;


        for(size_t n=1;n<=32768; n <<= 1)
        {
            vector<cplx> source(n,0);
            vector<cplx> fourier(n,0);
            support::fill1D(source);

            for(size_t i=n;i>0;--i) fourier[i] = source[i];

            FFT1::Forward_(fourier[1].re_prev(),n);
            FFT1::Reverse_(fourier[1].re_prev(),n);

            T rms = 0;
            for(size_t i=n;i>0;--i)
            {
                const cplx delta = fourier[i]/T(n)-source[i];
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
        
        FFT1::Forward<T,n>(fourier[1].re_prev());
        FFT1::Reverse<T,n>(fourier[1].re_prev());
        
        T rms = 0;
        for(size_t i=n;i>0;--i)
        {
            const cplx delta = fourier[i]/T(n)-source[i];
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
        std::cerr << "Specific test<" << type_name_of<T>() << ">" << std::endl;
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
        do_test2<T,65536>();


    }

    template <typename T>
    static inline void do_test_opt()
    {
        std::cerr << std::endl;
        std::cerr << "dispatched test<" << type_name_of<T>() << ">" << std::endl;
        typedef complex<T> cplx;

        for(size_t n=1;n<=65536; n <<= 1)
        {
            vector<cplx> source(n,0);
            vector<cplx> fourier(n,0);
            support::fill1D(source);

            for(size_t i=n;i>0;--i) fourier[i] = source[i];

            FFT1::Forward(fourier[1].re_prev(),n);
            FFT1::Reverse(fourier[1].re_prev(),n);

            T rms = 0;
            for(size_t i=n;i>0;--i)
            {
                const cplx delta = fourier[i]/T(n)-source[i];
                rms += delta.mod2();
            }
            rms /= n;
            rms = mkl::sqrt_of(rms);
            std::cerr << "rms#" << n << " = " << rms << std::endl;
        }
     }

    template <typename T>
    static inline void do_test_tmx(const double D)
    {
        std::cerr << std::endl;
        std::cerr << "Timings for<" << type_name_of<T>() << ">" << std::endl;
        typedef complex<T> cplx;

        for(size_t n=1;n<=65536; n <<= 1)
        {
            std::cerr << std::setw(8) << n << " : ";
            std::cerr.flush();

            vector<cplx> fourier(n,0);
            support::fill1D(fourier);

            double speed0 = 0;
            Y_TIMINGS(speed0,D,
                      FFT1::Forward_(fourier[1].re_prev(),n);
                      FFT1::Reverse_(fourier[1].re_prev(),n);
                      );
            double speed1 = 0;
            Y_TIMINGS(speed1,D,
                      FFT1::Forward(fourier[1].re_prev(),n);
                      FFT1::Reverse(fourier[1].re_prev(),n);
                      );

            std::cerr << speed1/speed0 << std::endl;
        }

    }


}

#include "y/string/convert.hpp"

Y_UTEST(fft1)
{

    for(size_t i=0;i<FFT1::sin_table_size;++i)
    {
        std::cerr << FFT1::positive_sinus[i] << "," << FFT1::negative_sinus[i] << std::endl;
    }

    do_test<float>();
    do_test<double>();
    
    do_tests<float>();
    do_tests<double>();

    do_test_opt<float>();
    do_test_opt<double>();

    double D = 0.1;
    if(argc>1)
    {
        D = string_convert::to<double>(argv[1],"D");
    }
    do_test_tmx<float> (D);
    do_test_tmx<double>(D);

    
}
Y_UTEST_DONE()

