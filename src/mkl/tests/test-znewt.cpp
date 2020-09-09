
#include "y/utest/run.hpp"
#include "y/mkl/root/znewt.hpp"
#include "y/mkl/fcn/djacobian.hpp"
#include "y/sequence/vector.hpp"

using namespace upsylon;
using namespace mkl;

static const double rho_s = 12.0192;
static const double d7out = 14.57;
static const double eps6  = 1.0/(1.0+rho_s*(1+1e-3*d7out));
static const double eps7  = 1.0-eps6;
static const double sigma = 1.003772;

namespace {

    struct mysys
    {
        double nu;
        double d7ini;

        inline void operator()( addressable<double> &F, const accessible<double> &X )
        {
            const double t2 = X[1];
            const double tk = X[2];

            const double t2p1  = 1.0 + t2;
            const double s2    = t2/t2p1;
            const double c2    = 1.0/t2p1;
            const double kappa = 1.0 + tk / 1000.0;


            F[1] = (1.0+1e-3*d7out)/(1.0+1e-3*d7ini) - (sigma*c2 + kappa*s2);
            F[2] = (nu-1.0) - (kappa*eps6+eps7)/(sigma*eps6+eps7) * t2;
        }

    };


}

Y_UTEST(znewt)
{

    mysys                         mySys = { 30, 2 };
    numeric<double>::vector_field f     = mySys;
    derivative<double>::pointer   drvs  = new derivative<double>();
    djacobian<double>             fjac(f,drvs);

    vector<double> F(2,0);
    vector<double> X(2,0);

    // initialize
    X[1] = 30;
    X[2] = 10;
    f(F,X);
    znewt<double> solver;

    //! run
    while(!solver.step(F,X,f,fjac))
    {

    }
    std::cerr << std::endl;
    std::cerr << "kappa = " << 1.0 + X[2] / 1000.0 << std::endl;
    std::cerr << "t2    = " << X[1] << std::endl;


}
Y_UTEST_DONE()


