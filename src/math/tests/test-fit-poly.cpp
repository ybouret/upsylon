
#include "y/utest/run.hpp"
#include "y/sort/heap.hpp"
#include "y/math/fit/polynomial.hpp"
#include "y/ios/ocstream.hpp"
#include "y/math/fit/ls.hpp"

using namespace upsylon;
using namespace math;

Y_UTEST(fit_poly)
{


    vector<double> X;
    vector<double> Y;

    const double phi = 0.1*alea.symm<double>();
    for( size_t i=30+alea.leq(100); i>0; --i)
    {
        const double x = 3.5 * alea.to<double>();
        const double y = sin(x+phi) + 0.02 * alea.symm<double>();
        X.push_back(x);
        Y.push_back(y);
    }

    hsort(X,Y,comparison::increasing<double>);
    const size_t N = X.size();


    {
        ios::ocstream fp("poly.dat");
        for(size_t i=1;i<=N;++i) fp("%g %g\n", X[i], Y[i]);
    }

    Fit::Type<double>::CFunction f = Fit::Polynomial::Compute<double>;
    Fit::Type<double>::Function F(f);
    Fit::LeastSquares<double>   ls(true);

    matrix<double> Z(4,N);
    for(size_t n=1;n<=Z.rows;++n)
    {
        Fit::Sample<double> sample(X,Y,Z[n]);
        vector<double> aorg(n);
        Fit::Polynomial::Initialize(aorg,sample);
        std::cerr << "aorg=" << aorg << std::endl;
    }
    
}
Y_UTEST_DONE()

