
#include "y/mkl/fitting/lambda.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/mkl/types.hpp"

using namespace upsylon;
using namespace mkl;
using namespace fitting;

namespace
{
    template <typename T>
    static inline void show_lambda( const lambdas<T> &l )
    {
        std::cerr << "pmin=" << l.pmin << std::endl;
        std::cerr << "pmax=" << l.pmax << std::endl;
        Y_CHECK(fabs_of(l[l.pmin])<=0);
        for(int p=l.pmin;p<=l.pmax;++p)
        {
            std::cerr << "l[" << p << "]=" << l[p] << std::endl;
        }
    }
}

Y_UTEST(fitting_lam)
{

    lambdas<double> ld; show_lambda(ld);

    lambdas<float>  lf; show_lambda(lf);

}
Y_UTEST_DONE()
