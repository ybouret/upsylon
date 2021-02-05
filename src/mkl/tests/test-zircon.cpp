#include "y/mkl/solve/zircon.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/type/spec.hpp"

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
    void runZircon(const T y0)
    {
        const string trace = "zircon-" + type_name_of<T>()  + ".dat";
        ios::ocstream::overwrite(trace);
        vector<T> X(2,0);

        for(size_t iter=0;iter<80;++iter)
        {
            X[1] = 2*alea.symm<T>();
            X[2] = 2*alea.symm<T>();

            fcn<T> f    = { y0 };
            jac<T> fjac;

            Zircon<T> zrc(true);
            int       p  = zrc.pmin+3;
            T         g0 = -1;
            const zircon::status result = zrc.cycle(X,f,fjac,p,g0,*trace);
            ios::ocstream::echo(trace,"\n");
            std::cerr << "\t" << zircon::readable(result) << std::endl;
            std::cerr << "\t" << g0 << "@" << X << std::endl;
        }

    }

}

#include "y/string/convert.hpp"

Y_UTEST(zircon)
{
    float y0 = 0.5f;

    if(argc>1)
    {
        y0 = string_convert::to<float>(argv[1],"y0");
    }
    runZircon<float>(y0);
    runZircon<double>(y0);

}
Y_UTEST_DONE();


