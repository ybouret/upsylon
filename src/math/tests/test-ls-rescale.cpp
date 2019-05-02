
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
        return 0.1 * sin( 3 * (x + 0.1) );
    }

    double g1d( double x )
    {
        return sin(x);
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

    Fit::Rescaling<double>    rs;
    Fit::LeastSquares<double> ls;

    rs.use_coeff() = true;
    
    if(!rs.update(ls,sample,g1d,Fit::Rescale::StartWithIdentity))
    {
        throw exception("no rescaling");
    }
    rs.vars.display(std::cerr,rs.values(),rs.errors());

    rs.use_scale() = true;
    if(!rs.update(ls,sample,g1d,Fit::Rescale::WouldTakeNextStep))
    {
        throw exception("no rescaling");
    }
    rs.vars.display(std::cerr,rs.values(),rs.errors());

    rs.use_shift() = true;
    if(!rs.update(ls,sample,g1d,Fit::Rescale::WouldTakeNextStep))
    {
        throw exception("no rescaling");
    }
    rs.vars.display(std::cerr,rs.values(),rs.errors());

    
}
Y_UTEST_DONE()

