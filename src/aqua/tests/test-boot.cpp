

#include "y/aqua/solver.hpp"
#include "y/aqua/boot.hpp"
#include "y/aqua/library.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;
using namespace Aqua;

Y_UTEST(boot)
{
    Library lib;

    Species &h   = lib("H+",1);
    Species &w   = lib("HO-",-1);
    Species &ah  = lib("AH",0);
    Species &am  = lib("Am",-1);
    Species &NH4 = lib("NH4+",1);
    Species &NH3 = lib("NH3",0);

    Species &Na = lib("Na+",1);  (void)Na;
    Species &Cl = lib("Cl-",-1); (void)Cl;

    std::cerr << lib << std::endl;

    Equilibria eqs;
    {
        Equilibrium &water = eqs.constant("water",1e-14);
        water(h,1);
        water(w,1);
    }

    {
        Equilibrium &acid = eqs.constant("acid",pow(10.0,-4.8));
        acid(ah,-1);
        acid(am,1);
        acid(h,1);
    }

    {
        Equilibrium &ammoniac = eqs.constant("ammoniac",pow(10.0,-9.2));
        ammoniac(NH4,-1);
        ammoniac(NH3,1);
        ammoniac(h,1);
    }



    std::cerr << eqs << std::endl;
    

    Boot boot;
    boot.electroneutrality(lib);
    boot.conserve(1e-8,ah,am);
    boot.conserve(0.000,NH4,NH3);
    boot.conserve(0,Na);
    boot.conserve(0.000,Cl);
    boot.init(lib);
    
    std::cerr << "R=" << boot.R << std::endl;
    std::cerr << "d=" << boot.d << std::endl;
    std::cerr << "F=" << boot.F << std::endl;
    std::cerr << "S=" << boot.S << std::endl;

    

    return 0;

    Solver cs;
    cs.init(lib,eqs);

    cs.computeK(0);

    vector<double> C(cs.M+2,0);

    std::cerr << boot << std::endl;

    cs.balanceVerbose = true;
    cs.bootingVerbose = true;
    cs.boot(C,boot);


}
Y_UTEST_DONE()

#include "y/mkl/kernel/lu.hpp"
using namespace mkl;
Y_UTEST(r2i)
{
    const double xx[] =
    {
        0.616815,
        0.755733,
        -0.0158463,
        0.0158463,
        0.154765,
        -0.154765,
        0.0,
        0.0
    };
    const size_t n = sizeof(xx)/sizeof(xx[0]);

    const lightweight_array<double> X((double*)xx,n);

    std::cerr << "X=" << X << std::endl;

    vector<int64_t> x(n,0);
    for(size_t i=n;i>0;--i)
    {
        const double Xi = X[i];
        x[i] = (Xi<0) ? -1 : ( (0<Xi) ? 1 : 0 );
    }

    matrix<double> J(n,n);
    vector<double> F(n,0);
    vector<double> dx(n,0);

    std::cerr << "x=" << x << std::endl;

    int64_t x2 = 0;
    for(size_t i=n;i>0;--i) x2 += x[i] * x[i];
    const double den = sqrt( double(x2) );

    for(size_t i=n;i>0;--i)
    {
        F[i] = den * X[i] - x[i];
        for(size_t j=n;j>0;--j)
        {
            J[i][j] = (X[i] * x[j])/den;
        }
        J[i][i] -= 1.0;
        dx[i]    = -F[i];
    }
    std::cerr << "J=" << J << std::endl;
    std::cerr << "F=" << F << std::endl;
    if( !LU::build(J) )
    {
        throw exception("singular");
    }

    LU::solve(J,dx);
    std::cerr << "dx" << dx << std::endl;

}
Y_UTEST_DONE()

