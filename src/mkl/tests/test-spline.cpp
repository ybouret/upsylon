
#include "y/mkl/signal/spline.hpp"
#include "y/utest/run.hpp"

#include "y/sequence/vector.hpp"
#include "y/ios/ocstream.hpp"

using namespace upsylon;


Y_UTEST(spline)
{
    vector<double> X;
    vector<double> Y;

    {
        ios::ocstream fp("spldat.dat");
        double x = 0.2 * alea.symm<double>();
        for(size_t n=10+alea.leq(30);n>0;--n)
        {
            x += 0.2 * alea.to<double>();
            X << x;
            Y << sin(x) + 0.01 * alea.symm<double>();
            fp("%g %g\n", X.back(), Y.back());
        }
    }

    vector<double> Y2n(Y.size());
    vector<double> Y2l(Y.size());
    vector<double> Y2u(Y.size());
    vector<double> Y2b(Y.size());
    vector<double> Y2c(Y.size());

    math::spline::compute<double>(Y2n, X, Y, true, 0,             true, 0);
    math::spline::compute<double>(Y2l, X, Y, false,cos(X.front()),true, 0);
    math::spline::compute<double>(Y2u, X, Y, true, 0,             false,cos(X.back()) );
    math::spline::compute<double>(Y2b, X, Y, false,cos(X.front()),false,cos(X.back()));
    math::spline::compute<double>(Y2b, X, Y, false,0,false,0);



#if 1
    {
        ios::ocstream fp("spline.dat");
        for(double x=X.front()-0.5;x<=X.back()+0.5;x+=0.001)
        {
            fp("%g %g %g %g %g %g\n",x,
               math::spline::eval(x,X,Y,Y2n),
               math::spline::eval(x,X,Y,Y2l),
               math::spline::eval(x,X,Y,Y2u),
               math::spline::eval(x,X,Y,Y2b),
               math::spline::eval(x,X,Y,Y2c)
               );
        }
    }
#endif

}
Y_UTEST_DONE()

