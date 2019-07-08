
#include "y/math/fit/explode.hpp"
#include "y/utest/run.hpp"
#include "y/ios/ocstream.hpp"
#include "y/math/fit/samples-io.hpp"
#include "y/math/fit/ls.hpp"

using namespace upsylon;
using namespace math;

namespace
{
    class Shape : public ODE::System<double>
    {
    public:
        explicit Shape() throw() {}
        virtual ~Shape() throw() {}
        
        virtual  size_t dimension() const throw()
        {
            return 2;
        }
        
        virtual void setup( Array &y, const Array &, const Variables &) const throw()
        {
            y[1] = 1;
            y[2] = 0;
        }
        
        virtual void rates( array<double> &dydx,
                           double ,
                           const array<double> &y,
                           const array<double> &aorg,
                           const Variables     &vars)
        {
            assert(dydx.size()==2);
            assert(y.size()==2);
            
            const double omega  = vars(aorg,"omega");
            const double lambda = vars(aorg,"lambda");

            dydx[1] = y[2];
            dydx[2] = - omega*omega * y[1] - lambda * y[2];
        }
        
        virtual double start() const throw()
        {
            return 0;
        }
        
        virtual double delta() const throw()
        {
            return 0.1;
        }
        
        virtual double query( const double, const array<double> &y, const array<double> &, const Variables & )
        {
            return y[1];
        }
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(Shape);
        
    };
}

Y_UTEST(lsf_ode)
{
    
    Shape                                      shape;
    {
        const ODE::ExplicitSolver<double>::Pointer driver0 = ODE::DriverCK<double>::New();
        Fit::ExplODE<double>                       F0(driver0,shape);
        F0->eps = 1e-3;
    }

    Fit::ExplODE<double>  F(shape);
    F->eps = 1e-3;
   
    
    const size_t NP    = 50 + alea.leq(50);
    const double range = 5+alea.leq(10);
    double       noise = 0.1;
    
    vector<double> X,Y,Yf;
    Fit::Sample<double> sample(X,Y,Yf);
    
    Fit::Variables &vars = sample.variables; vars << "omega" << "lambda";
    const size_t    nvar = vars.size();
    std::cerr << "vars=" << vars << std::endl;
    vector<double> aorg(nvar,0);
    vector<double> aerr(nvar,0);
    vector<bool>   used(nvar,0);
    
    double &omega  = vars(aorg,"omega");
    double &lambda = vars(aorg,"lambda");
    omega  = 8.0;
    lambda = 0.3;

    for(size_t i=NP;i>0;--i)
    {
        X.push_back( range * alea.to<double>() );
        Y.push_back( cos( omega * X.back()) * (1.0+noise*alea.symm<double>()) * exp(-lambda*X.back()) );
        Yf.push_back(0);
    }
    
    sample.prepare();
    omega  *= 0.9;
    lambda *= 1.1;

    std::cerr << "D2=" << sample.computeD2(F, aorg) << std::endl;
    Fit::IO::Save("ode-fit0.dat",sample,true);

    
    Fit::LeastSquares<double> ls;
    

    vars.on(used, "omega");
    
    ls.verbose = true;
    {
        if( !ls.fit(sample, F, aorg, aerr, used ) )
        {
            throw exception("couldn't fit");
        }
    }
    vars.display(std::cerr, aorg, aerr);
    Fit::IO::Save("ode-fit1.dat",sample,true);

    vars.on(used, "lambda");
    {
        if( !ls.fit(sample, F, aorg, aerr, used) )
        {
            throw exception("couldn't fit");
        }
    }

    vars.display(std::cerr, aorg, aerr);
    Fit::IO::Save("ode-fit2.dat",sample,true);


    {
        ios::ocstream fp("ode-func.dat");
        double y = F.initialize(0, aorg, vars);
        fp("%g %g\n", 0.0, y);
        double dx = 0.01;
        for(double x=dx;x<=range;x+=dx)
        {
            y = F.compute_to(x, aorg,vars);
            fp("%g %g\n", x, y);
        }
    }
    

}
Y_UTEST_DONE()

