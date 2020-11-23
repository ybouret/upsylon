#include "y/mkl/kernel/spark.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"

using namespace upsylon;
using namespace mkl;

namespace {

    template <typename T>
    static inline void fill(sparse_array<T> &a)
    {
        a.ldz();
        for(size_t i=1+alea.leq(a.size()/2);i>0;--i)
        {
            const T tmp = support::get<T>();
            a[ 1 + alea.lt(a.size()) ] = tmp;
        }
    }

    template <typename T>
    static inline void fill(sparse_matrix<T> &M)
    {
        for(size_t i=1+alea.leq(M.count)/4;i>0;--i)
        {
            const T tmp = support::get<T>();
            M( alea.range<size_t>(1,M.rows), alea.range<size_t>(1,M.cols) ) = tmp;
        }
    }

    template <typename T>
    static inline void do_spark()
    {
        {
            const size_t    n = 1 + alea.leq(20);
            sparse_array<T> a(n); fill(a);
            sparse_array<T> b(n); fill(b);
            std::cerr << "a     = " << a << std::endl;
            std::cerr << "b     = " << b << std::endl;
            spark::add(a,b);
            std::cerr << "a+b   = " << a << std::endl;
            spark::sub(a,b);
            std::cerr << "a-b   = "   << a << std::endl;
            std::cerr << "a     = "   << a << std::endl;
            std::cerr << "a.b   = "   << spark::dot(a,b) << std::endl;
            std::cerr << "|a|^2 = "   << spark::norm2(a) << std::endl;

            fill(a);
            fill(b);
            spark::set(a,b);
            
            const T x = support::get<T>();
            fill(a);
            fill(b);
            std::cerr << "a=" << a << std::endl;
            std::cerr << "b=" << b << std::endl;
            std::cerr << "x=" << x << std::endl;
            spark::mul_add(a,x,b);
            std::cerr << "c=" << a << std::endl;
            spark::mul_sub(a,x,b);
            std::cerr << "d=" << a << std::endl;
        }

        {

        }

        {
            const size_t r = 1 + alea.leq(10);
            const size_t c = 1 + alea.leq(10);
            sparse_matrix<T> M(r,c); fill(M);
            sparse_array<T>  b(c);   fill(b);
            sparse_array<T>  a(r);   fill(a);

            spark::mul(a,M,b);
            std::cerr << "M=" << M << std::endl;
            std::cerr << "b=" << b << std::endl;
            std::cerr << "a=" << a << std::endl;
        }

        {
            const size_t r = 1 + alea.leq(10);
            const size_t c = 1 + alea.leq(10);
            sparse_matrix<T> M(c,r); fill(M);
            sparse_array<T>  b(c);   fill(b);
            sparse_array<T>  a(r);   fill(a);

            spark::mul_trn(a,M,b);
            std::cerr << "M=" << M << std::endl;
            std::cerr << "b=" << b << std::endl;
            std::cerr << "a=" << a << std::endl;
        }

    }


}

Y_UTEST(sparse)
{
    do_spark<uint8_t>();
    do_spark<float>();
    do_spark<double>();
    do_spark<int>();
    do_spark<apz>();
    do_spark<apq>();
}
Y_UTEST_DONE()


