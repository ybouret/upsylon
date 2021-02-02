#include "y/mkl/solve/zircon.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"

using namespace upsylon;
using namespace mkl;

namespace
{

    template <typename T>
    struct fcn
    {
        T y0;

        void operator()(addressable<T> &F, const accessible<T> &X)
        {
            const T x = X[1];
            const T y = X[2];
            F[1] = sqrt_of(square_of(x)+square_of(y))-1;
            F[2] = (y-(x+y0));
        }
    };

    template <typename T>
    struct jac
    {

        void operator()(matrix<T> &J, const accessible<T> &X)
        {
            const T x = X[1];
            const T y = X[2];
            const T r = sqrt_of( numeric<T>::tiny + x*x + y*y);

            J[1][1] = x/r; J[1][2] = y/r;

            J[2][1] = -1; J[2][2] = 1;
        }
    };

    template <typename T>
    void runZircon()
    {
        vector<T> X(2,0);
        X[1] = alea.symm<T>();
        X[2] = alea.symm<T>();

        fcn<T> f    = { 0.5 };
        jac<T> fjac;

        zircon<T> zrc(true);

        zrc.cycle(X,f,fjac);

    }

}

Y_UTEST(zircon)
{

    runZircon<float>();
}
Y_UTEST_DONE();


