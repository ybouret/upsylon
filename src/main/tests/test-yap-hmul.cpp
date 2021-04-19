
#include "y/utest/run.hpp"
#include "y/utest/timings.hpp"

#include "y/type/complex.hpp"
#include "y/sequence/vector.hpp"
#include "support.hpp"
#include "y/type/spec.hpp"
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
                Y_H_MUL(1); Y_H_MUL(2); Y_H_MUL(4); Y_H_MUL(8); Y_H_MUL(16);
                Y_H_MUL(32); Y_H_MUL(64); Y_H_MUL(128); Y_H_MUL(256); Y_H_MUL(512);

            default:
                for(size_t i=0;i<N;++i) L[i] *= R[i];
        }
    }

    template <typename T>
    static inline void do_test()
    {
        std::cerr << "h-mul with [" << type_name_of<T>() << "]" << std::endl;
        for(size_t p=0;p<=16;++p)
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
        }
        std::cerr << std::endl;
    }

}

Y_UTEST(yap_hmul)
{
    do_test<float>();
    do_test< complex<double> >();

}
Y_UTEST_DONE()
