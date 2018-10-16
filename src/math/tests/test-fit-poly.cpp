
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
    Fit::Type<double>::Function  F(f);
    Fit::LeastSquares<double>    ls(false);


    matrix<double> Z(4,N);
    matrix<double> Zf(Z.rows,N);
    for(size_t n=1;n<=Z.rows;++n)
    {
        Fit::Sample<double> sample(X,Y,Zf[n]);
        Fit::Polynomial::Create(sample.variables,n);
        vector<double>      aorg(n);
        vector<double>      aerr(n);
        vector<bool>        used(n,true);
        Fit::Polynomial::Initialize(aorg,sample);
        std::cerr << "aorg=" << aorg << std::endl;
        for(size_t i=1;i<=N;++i)
        {
            Z[n][i] = polynomial::eval(X[i],aorg);
        }
        if( !ls.fit(sample,F,aorg,aerr,used) )
        {
            throw exception("Couldn't poly-fit n=%u", unsigned(n));
        }
        sample.variables.diplay(std::cerr,aorg,aerr);
        
    }

    {
        ios::ocstream fp("poly0.dat");
        for(size_t i=1;i<=N;++i)
        {
            fp("%g",X[i]);
            for(size_t j=1;j<=Z.rows;++j)
            {
                fp(" %g", Z[j][i]);
            }
            fp << "\n";
        }
    }

    {
        ios::ocstream fp("poly1.dat");
        for(size_t i=1;i<=N;++i)
        {
            fp("%g",X[i]);
            for(size_t j=1;j<=Z.rows;++j)
            {
                fp(" %g", Zf[j][i]);
            }
            fp << "\n";
        }
    }
}
Y_UTEST_DONE()

