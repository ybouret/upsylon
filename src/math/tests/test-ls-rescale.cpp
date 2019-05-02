
#include "y/math/fit/ls-rescale.hpp"
#include "y/math/fit/vectors.hpp"
#include "y/utest/run.hpp"
#include "y/ios/ocstream.hpp"

using namespace upsylon;
using namespace math;

namespace
{

    double f1d( double x )
    {
        return 0.1 * sin( 3 * x + 0.1 );
    }

}

Y_UTEST(ls_rescale)
{

    Fit::Vectors<double> vec("vec");

    const size_t N = 10 + alea.leq(100);
    vec.X.make(N,0);
    vec.Y.make(N,0);
    vec.adjust();

    for(size_t i=1;i<=N;++i)
    {
        vec.X[i] = alea.symm<double>();
        vec.Y[i] = f1d( vec.X[i] );
    }

    Fit::Sample<double> sample(vec.X,vec.Y,vec.Yf);

    numeric<double>::function F = cfunctor(f1d);
    Fit::Rescaler<numeric<double>::function> rs( F );


    Fit::Variables &vars = sample.variables;
    rs.declare( sample.variables );
    const size_t nv = vars.size();
    vector<double> aorg(nv,0);
    vector<double> aerr(nv,0);
    vector<bool>   used(nv,false);

    rs.initialize(aorg,vars);




    
}
Y_UTEST_DONE()

