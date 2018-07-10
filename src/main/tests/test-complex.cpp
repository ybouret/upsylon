#include "y/type/complex.hpp"
#include "y/utest/run.hpp"
#include <typeinfo>

using namespace upsylon;

namespace
{
    template <typename T>
    static inline void do_test()
    {
        typedef complex<T> cplx;
        std::cerr << "real_type=" << typeid(typename cplx::real_type).name() << std::endl;
        std::cerr << "sizeof(cplx)=" << sizeof(cplx) << std::endl;
        std::cerr << "-- set" << std::endl;
        cplx a;       std::cerr << a << std::endl;
        cplx b(1);    std::cerr << b << std::endl;
        cplx c(1,2);  std::cerr << c << std::endl;
        b = 2;        std::cerr << b << std::endl;
        b = c;        std::cerr << b << std::endl;
        std::cerr << "-- add" << std::endl;
        b += c;       std::cerr << b << std::endl;
        a = b+c;      std::cerr << a << std::endl;
        a = b + 1;    std::cerr << a << std::endl;
        a = 1 + b;    std::cerr << a << std::endl;
        std::cerr << "-- sub" << std::endl;
        b = -a;       std::cerr << b << std::endl;
        b += c;       std::cerr << b << std::endl;
        b = c+3;      std::cerr << b << std::endl;
        b = 3+c;      std::cerr << b << std::endl;
        std::cerr << "-- mul" << std::endl;
        b *= a;       std::cerr << b << std::endl;
        b  = a*c;     std::cerr << b << std::endl;
        b  = a*2;     std::cerr << b << std::endl;
        b  = 2*a;     std::cerr << b << std::endl;
        std::cerr << "-- conj" << std::endl;
        std::cerr << b.conj() << std::endl;
        std::cerr << "-- div" << std::endl;
        b /= a;       std::cerr << b << std::endl;
        b += cplx(0,1);
        b /= 2;       std::cerr << b << std::endl;
        b  = a/c;     std::cerr << b << std::endl;
        b  = a/2;     std::cerr << b << std::endl;
        c  = 2/a;     std::cerr << c << std::endl;
        cplx d = b*c; std::cerr << d << std::endl;
    }

}

Y_UTEST(complex)
{
    do_test<float>();
    do_test<double>();
}
Y_UTEST_DONE()


