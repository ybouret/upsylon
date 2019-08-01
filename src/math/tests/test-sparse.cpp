#include "y/math/kernel/sparse.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"

using namespace upsylon;
using namespace math;

namespace {

    template <typename T>
    static inline void fill(sparse_array<T> &a)
    {
        for(size_t i=1+alea.leq(a.size()/2);i>0;--i)
        {
            const T tmp = support::get<T>();
            a[ 1 + alea.lt(a.size()) ] = tmp;
        }
    }

    template <typename T>
    static inline void do_spark()
    {
        const size_t    n = 1 + alea.leq(20);
        sparse_array<T> a(n); fill(a);
        sparse_array<T> b(n); fill(b);
        std::cerr << "a   = " << a << std::endl;
        std::cerr << "b   = " << b << std::endl;
        spark::add(a,b);
        std::cerr << "a+b = " << a << std::endl;
        spark::sub(a,b);
        std::cerr << "a-b = " << a << std::endl;
        std::cerr << "a   = " << a << std::endl;

    }


}

Y_UTEST(sparse)
{
    do_spark<float>();
    do_spark<double>();
    do_spark<int>();
    do_spark<mpz>();
    do_spark<mpq>();

}
Y_UTEST_DONE()


