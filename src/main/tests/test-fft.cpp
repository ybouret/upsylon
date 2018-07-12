#include "y/fft/fft.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"

using namespace upsylon;

namespace
{
    template <typename T>
    static inline
    void do_test()
    {
        std::cerr << "sizeof(T)=" << sizeof(T) << std::endl;
#if 0
        for(size_t i=0;i<sizeof(fft<T>::sin_table)/sizeof(fft<T>::sin_table[0]);++i)
        {
            std::cerr << "sin_table[" << i << "]=" <<fft<T>::sin_table[i] << std::endl;
        }
#endif
        typedef complex<T> cplx;

        for(size_t n=1;n<=16384;n*=2)
        {
            vector<cplx> U(n);
            for(size_t i=1;i<=n;++i) { U[i].re = alea.to<T>(); U[i].im = alea.to<T>(); }
            vector<cplx> V;
            //std::cerr << "U=" << U << std::endl;
            V = U;
            fft<T>::forward(&(V[1].re)-1,V.size());
            //std::cerr << "V=" << V << std::endl;
            fft<T>::reverse(&(V[1].re)-1,V.size());
            for(size_t i=n;i>0;--i) V[i]/=n;
            //std::cerr << "W=" << V << std::endl;
            double rms = 0;
            for(size_t i=n;i>0;--i)
            {
                rms += (U[i]-V[i]).mod2();
            }
            rms = sqrt(rms/n);
            std::cerr << "rms=" << rms << "\t/ " << n << std::endl;
        }


    }
}

Y_UTEST(fft)
{
    do_test<float>();
    do_test<double>();
}
Y_UTEST_DONE()


