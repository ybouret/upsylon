
#include "y/utest/run.hpp"
#include "y/utest/timings.hpp"

#include "y/type/complex.hpp"
#include "y/sequence/vector.hpp"
#include "support.hpp"
#include "y/type/spec.hpp"
#include "y/code/hr-ints.hpp"
#include <cstring>

using namespace upsylon;

namespace
{

    template <typename T, const size_t N> struct h_mul
    {
        static const size_t H = (N>>1);
        static inline void run(T *L, const T *R) throw()
        {
            h_mul<T,H>::run(L,R);
            h_mul<T,H>::run(L+H,R+H);
        }
    };

    template <typename T> struct h_mul<T,1>
    {
        static inline void run(T *L, const T *R) throw()
        {
            L[0] *= R[0];
        }
    };

    template <typename T> static inline
    void h_safe(T *L, const T *R, const size_t N) throw()
    {
        for(size_t i=0;i<N;++i) L[i] *= R[i];
    }


#define Y_H_MUL(N) case N: h_mul<T,N>::run(L,R); return
    template <typename T>
    void h_vmul(T *L, const T *R, const size_t N) throw()
    {
        switch(N)
        {
                Y_H_MUL(1);  Y_H_MUL(2);  Y_H_MUL(4);   Y_H_MUL(8);   Y_H_MUL(16);
                Y_H_MUL(32); Y_H_MUL(64); Y_H_MUL(128); Y_H_MUL(256); Y_H_MUL(512);
                Y_H_MUL(1024); Y_H_MUL(2048);

            default:
                for(size_t i=0;i<N;++i) L[i] *= R[i];
        }
    }

    template <typename T>
    static inline void do_test(const double D)
    {
        std::cerr << "h-mul with [" << type_name_of<T>() << "]" << std::endl;
        for(size_t p=0;p<=10;++p)
        {
            const size_t n = size_t(1) << p;
            vector<T>    lhs(n,0), rhs(n,0), seq(n,0);
            for(size_t i=n;i>0;--i)
            {
                lhs[i] = seq[i] = support::get<T>();
                rhs[i] = support::get<T>();
            }

            h_vmul(*lhs,*rhs,n);
            h_safe(*seq,*rhs,n);
            std::cerr << std::setw(8) << n << ' '; Y_CHECK( 0 == memcmp(*lhs,*seq,n*sizeof(T)));

            uint64_t        vmul = 0;
            uint64_t        safe = 0;
            real_time_clock clk;
            uint64_t        cycles = 0;
            do
            {
                ++cycles;
#if 0
                for(size_t i=n;i>0;--i)
                {
                    lhs[i] = seq[i] = support::get<T>();
                    rhs[i] = support::get<T>();
                }
#endif

                Y_RTC_ADD(vmul,  h_vmul(*lhs,*rhs,n) );
                Y_RTC_ADD(safe,  h_safe(*seq,*rhs,n) );


            } while( clk(vmul)<D && clk(safe)<D );
            const int64_t vmul_speed = int64_t( floor( double(cycles)/clk(vmul) + 0.5) );
            const int64_t safe_speed = int64_t( floor( double(cycles)/clk(safe) + 0.5) );
            std::cerr << std::setw(8) << '|' << '-';
            std::cerr << " vmul: " << human_readable(vmul_speed);
            std::cerr << " safe: " << human_readable(safe_speed);
            std::cerr << " rho : " << double(vmul_speed)/double(safe_speed);

            std::cerr << std::endl;
        }
        std::cerr << std::endl;
    }



}

Y_UTEST(yap_hmul)
{
    double D = 0.2;
    do_test<float>(D);
    do_test< complex<double> >(D);

}
Y_UTEST_DONE()
