#include "y/math/fit/ls.hpp"
#include "y/math/fit/vectors.hpp"
#include "y/utest/run.hpp"
#include "y/sort/heap.hpp"
#include "y/ios/ocstream.hpp"

using namespace upsylon;
using namespace math;

namespace
{
    static inline double build_sample( const double A, array<double> &X, array<double> &Y )
    {
        assert(X.size()==Y.size());
        const size_t n = X.size();
        const double phi = alea.to<double>() * numeric<double>::two_pi;

        for(size_t i=1;i<=n;++i)
        {
            X[i] = alea.to<double>() * 10.0;
            Y[i] = A * cos( X[i] + phi ) + (0.05-0.1*alea.to<double>());
        }

        hsort(X,Y,comparison::increasing<double>);

        return phi;
    }

    struct  dummy
    {
        double f( double x, const array<double> &a, const Fit::Variables &var)
        {
            const double A   = var(a,"A");
            const double phi = var(a,"phi");
            return A*cos( x + phi);
        }

    };

}

Y_UTEST(lsf2)
{
    // Organizing data
    Fit::VectorsDB<double> vdb(2);
    Fit::Vectors<double>  &v1 = vdb.create("values1",40);
    Fit::Vectors<double>  &v2 = vdb.create("values2",50);



    Fit::Samples<double> samples(4);

    Fit::Sample<double> &sample1 = v1.add_to(samples);
    Fit::Sample<double> &sample2 = v2.add_to(samples);

    Fit::Variables &vars = samples.variables;

    vars << "A" << "phi1" << "phi2";

    sample1.variables("A", vars["A"])("phi", vars["phi1"]);

    sample2.variables("A", vars["A"])("phi", vars["phi2"]);

    std::cerr << "global vars=" << vars << std::endl;
    std::cerr << "local vars1=" << sample1.variables << std::endl;
    std::cerr << "local vars2=" << sample2.variables << std::endl;

    const double A    = 1 + 3 * alea.to<double>();
    const double phi1 = build_sample(A, v1.X, v1.Y);
    const double phi2 = build_sample(A, v2.X, v2.Y);

    const size_t   nv = vars.size();
    vector<double> aorg(nv);
    vector<double> aerr(nv);
    vector<bool>   used(nv,true);

    vars(aorg,"A")     = A;
    vars(aorg,"phi1") = phi1;
    vars(aorg,"phi2") = phi2;

    std::cerr << "initial: " << std::endl;
    vars.display(std::cerr,aorg);

    Fit::LeastSquares<double> LS;
    dummy dum;
    Fit::LeastSquares<double>::Function F( &dum, & dummy::f );

    if(!LS.fit(samples, F, aorg, aerr, used))
    {
        throw exception("couldn't fit!!!");
    }

    std::cerr << "final: " << std::endl;

    vars.display(std::cerr,aorg,aerr);

    {
        ios::ocstream fp("lsf2.dat");
        for(size_t i=1;i<=samples.size();++i)
        {
            const Fit::Sample<double> &s = *samples[i];
            for(size_t j=1;j<=s.X.size();++j)
            {
                fp("%g %g %g %u\n", s.X[j], s.Y[j], s.Yf[j], unsigned(i));
            }
            fp << '\n';
        }
    }



}
Y_UTEST_DONE()

