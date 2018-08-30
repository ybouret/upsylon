#include "y/math/fit/gaussians.hpp"
#include "y/utest/run.hpp"
#include "y/sort/heap.hpp"
#include "y/math/fit/ls.hpp"
#include "y/ios/ocstream.hpp"

using namespace upsylon;
using namespace math;

Y_UTEST(fit_gauss)
{

    vector<double> X;
    vector<double> Y;

    const double     a1   = 2.0;
    gaussian<double> g1(2.1,0.5);

    const double     a2   = 3.2;
    gaussian<double> g2(6.7,0.7);

    for( size_t i=30+alea.leq(100); i>0; --i)
    {
        const double x = 10 * alea.to<double>();
        const double y = a1 * g1(x) + a2 * g2(x) + 0.02 * alea.symm<double>();
        X.push_back(x);
        Y.push_back(y);
    }


    hsort(X, Y, comparison::increasing<double> );
    const size_t N = X.size();
    vector<double> Z(N);
    {
        ios::ocstream fp("gauss.dat");
        for(size_t i=1;i<=N;++i)
        {
            fp("%g %g\n", X[i], Y[i]);
        }
    }

    Fit::Sample<double> sample(X,Y,Z);

    Fit::Gaussians::Create(sample.variables,2);

    vector<double> aorg( sample.variables.size() );
    vector<double> aerr( aorg.size() );
    vector<bool>   used( aorg.size(), true );

    aorg[1] = 1;
    aorg[2] = 2;
    aorg[3] = 0.4;


    aorg[4] = 1;
    aorg[5] = 7;
    aorg[6] = 0.4;

    Fit::Type<double>::CFunction f = Fit::Gaussians::Compute<double>;
    Fit::Type<double>::Function F(f);
    Fit::LeastSquares<double> ls(true);
    if( ls.fit(sample,F,aorg,aerr,used) )
    {
        sample.variables.diplay(std::cerr,aorg,aerr);
        ios::ocstream fp("gaussf.dat");
        const size_t M = 200;
        for(size_t i=1;i<=M;++i)
        {
            const double x = i*(X[N]-X[1])/double(M);
            const double y = F(x,aorg,sample.variables);
            fp("%g %g\n",x,y);
        }
    }
    else
    {
        std::cerr << "[FAILURE]" << std::endl;
    }


}
Y_UTEST_DONE()


