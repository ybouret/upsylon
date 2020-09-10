
#include "y/utest/run.hpp"
#include "y/mkl/root/znewt.hpp"
#include "y/mkl/fcn/djacobian.hpp"
#include "y/sequence/vector.hpp"
#include "y/sort/sequence.hpp"

using namespace upsylon;
using namespace mkl;

static const double rho_s = 12.0192;
static const double d7out = 14.57;
static const double eps6  = 1.0/(1.0+rho_s*(1+1e-3*d7out));
static const double eps7  = 1.0-eps6;
static const double sigma = 1.003772;

static const double scale = 1000.0;

namespace {


    struct mysys
    {
        double nu;
        double d7ini;

        inline void compute( addressable<double> &F, const accessible<double> &X )
        {
            const double t2 = X[1];
            const double tk = X[2];

            const double t2p1  = 1.0 + t2;
            const double s2    = t2/t2p1;
            const double c2    = 1.0/t2p1;
            const double kappa = 1.0 + tk / scale;


            F[1] = (1.0+1e-3*d7out)/(1.0+1e-3*d7ini) - (sigma*c2 + kappa*s2);
            F[2] = (nu-1.0) - (kappa*eps6+eps7)/(sigma*eps6+eps7) * t2;
        }

    };


    static inline
    double solve(addressable<double> &F,
                 addressable<double> &X,
                 numeric<double>::vector_field &f,
                 jacobian<double>              &fjac)
    {
        znewt<double> solver;
        solver.verbose = true;
        f(F,X);
        size_t count = 0;
        while(!solver.cycle(F,X,f,fjac))
        {
            if(++count>1000) break;
        }
        return X[2];
    }

}

#include "y/string/convert.hpp"



Y_UTEST(znewt)
{

    mysys                         mySys = { 2, 1 };
    if(argc>1)
    {
        mySys.nu    = string_convert::to<double>(argv[1],"nu");
    }

    if(argc>2)
    {
        mySys.d7ini = string_convert::to<double>(argv[2],"d7ini");
    }


    numeric<double>::vector_field f( &mySys, & mysys::compute);
    derivative<double>::pointer   drvs  = new derivative<double>();
    djacobian<double>             fjac(f,drvs);

    vector<double> F(2,0);
    vector<double> X(2,0);

    // initialize
    X[1] = 30;
    X[2] = 10;
    const double k0 = solve(F,X,f,fjac);
    {
        std::cerr << std::endl;
        std::cerr << "kappa = " << 1.0 + k0 / scale << std::endl;
        std::cerr << "t2    = " << X[1] << std::endl;
    }

    
    const double nu_fac[] = { 0.9, 1.1 };
    const double d7_fac[] = { 0.9, 1.1 };

    const double nu = mySys.nu;
    const double d7 = mySys.d7ini;

    vector<double> ks;
    for(size_t i=0;i<2;++i)
    {
        mySys.nu = nu * nu_fac[i];
        for(size_t j=0;j<2;++j)
        {
            mySys.d7ini = d7 * d7_fac[j];
            const double k = solve(F,X,f,fjac);
            ks.push_back(k);
        }
    }
    sort_sequence(ks, comparison::increasing<double> );
    std::cerr << "ks=" << ks << std::endl;
    const double dk = (ks.back()-ks.front())/2.0;
    std::cerr << "dk=" << dk << "/k0=" << k0 << std::endl;

    std::cerr << "kappa=" << 1.0 + k0/scale << " \\pm " << dk/scale << std::endl;




}
Y_UTEST_DONE()

#include "y/mkl/kernel/svd.hpp"
#include "y/sparse/matrix.hpp"

namespace {

    static inline void doZNL(const matrix<double> &J)
    {
        matrix<double> u = J;
        vector<double> w(2,0);
        matrix<double> v(2,2);

        Y_CHECK(svd::build(u,w,v));
        sparse_matrix<double> W(w);
        std::cerr << "J=" << J << std::endl;
        std::cerr << "u=" << u << std::endl;
        std::cerr << "w=" << W << std::endl;
        std::cerr << "v=" << v << std::endl;

    }

}

Y_UTEST(znl)
{
    matrix<double> J(2,2);
    J[1][1] = 0; J[1][2] = 0;
    J[2][1] = 1; J[2][2] = -1;

    doZNL(J);
    {
        const double r = alea.symm<double>();
        J[1][1] = r; J[1][2] = r;
    }
    doZNL(J);
    

}
Y_UTEST_DONE()

