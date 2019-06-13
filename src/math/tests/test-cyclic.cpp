#include "y/math/kernel/cyclic.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"


using namespace upsylon;
using namespace math;

template <typename T>
static inline void do_cyclic()
{
    for(size_t n=5;n<=5;++n)
    {
        cyclic<T> C(n,3);
        array<T>  &r = C[0];
        array<T>  &u = C[1];
        array<T>  &x = C[2];

        if(n==3)
        {
            std::cerr << "#count              = " << C.count              << std::endl;
            std::cerr << "#tridiag_extraneous = " << C.tridiag_extraneous << std::endl;
            std::cerr << "#cyclic_offset      = " << C.cyclic_offset      << std::endl;
            std::cerr << "#cyclic_extraneous  = " << C.cyclic_extraneous  << std::endl;
        }

        for(size_t i=C.size();i>0;--i)
        {
            const T a = support::get<T>();
            const T c = support::get<T>();
            const T b = T(1) + fabs_of(a) + fabs_of(c);
            C.a[i] = a;
            C.b[i] = b;
            C.c[i] = c;

            r[i]   = support::get<T>();
            u[i]   = 0;
            x[i]   = 0;
        }
        std::cerr << "C=" << C << std::endl;
        std::cerr << "r=" << r << std::endl;
        C.solve(u,r);
        std::cerr << "u=" << u << std::endl;
        C.mul(x,u);
        std::cerr << "x=" << x << std::endl;

    }
}

Y_UTEST(cyclic)
{

    do_cyclic<float>();
    //do_cyclic<double>();
    //do_cyclic< complex<float> >();


}
Y_UTEST_DONE()


