#include "y/mkl/signal/linear.hpp"
#include "y/utest/run.hpp"

#include "y/sequence/vector.hpp"
#include "y/ios/ocstream.hpp"

using namespace upsylon;


Y_UTEST(linear)
{
    vector<double> X;
    vector<double> Y;

    {
        ios::ocstream fp("lindat.dat");
        double x = 0.4 * alea.symm<double>();
        for(size_t n=10+alea.leq(150);n>0;--n)
        {
            x += 0.2 * alea.to<double>();
            X << x;
            Y << sin(x) + 0.01 * alea.symm<double>();
            fp("%g %g\n", X.back(), Y.back());
        }
    }

    {
        ios::ocstream fp("linear.dat");
        for(double x=X.front()-0.5;x<=X.back()+0.5;x+=0.001)
        {
            fp("%g %g\n",x,math::linear::eval(x,X,Y));
        }
    }

    vector<double> xh;
    math::linear::zfind<double>(xh,0.5,X,Y);
    std::cerr << "xh=" << xh << std::endl;
}
Y_UTEST_DONE()

