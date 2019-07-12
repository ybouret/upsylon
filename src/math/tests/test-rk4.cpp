
#include "y/math/ode/rk4.hpp"
#include "y/utest/run.hpp"
#include "y/ios/ocstream.hpp"
#include "y/sequence/vector.hpp"
#include "y/string.hpp"

using namespace upsylon;
using namespace math;

namespace {

    template <typename T>
    class dummy
    {
    public:
        T k;
        inline dummy() : k(T(1.7))
        {
        }

        inline ~dummy() throw()
        {
        }


        inline void eq1( array<T> &dydx, T, const array<T> &y)
        {
            dydx[1] = - k * y[1];
        }

        inline void eq2( array<T> &dydx, T, const array<T> &y)
        {
            dydx[1] = y[2];
            dydx[2] = -(k*k)*y[1];
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(dummy);
    };

    template <typename T>
    static inline void do_test()
    {
        dummy<T> D;
        typename ODE::Field<T>::Equation eq1( &D, & dummy<T>::eq1 );
        typename ODE::Field<T>::Equation eq2( &D, & dummy<T>::eq2 );

        ODE::RK4<T> rk4_1;
        ODE::RK4<T> rk4_2;

        const string  fn = vformat("rk4-%d.dat", int(sizeof(T)*8) );
        ios::ocstream fp(fn);
        vector<T> Y1(1),Y2(2);
        Y1[1] = 2;
        Y2[1] = 2;
        Y2[2] = 0;
        T x0 = 0;
        const T dx = T(1e-4);
        fp("%g %g %g\n",x0,Y1[1],Y2[1]);
        rk4_1.acquire(1);
        rk4_2.acquire(2);
        while(x0<5)
        {
            const T x1 = x0+dx;
            rk4_1(eq1,Y1,x0,x1,0);
            rk4_2(eq2,Y2,x0,x1,0);

            x0=x1;
            fp("%g %g %g\n",x0,Y1[1],Y2[1]);
        }
    }
}

Y_UTEST(rk4)
{
    do_test<float>();
    do_test<double>();
}
Y_UTEST_DONE()
