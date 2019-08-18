
#include "y/math/ode/explicit/explode.hpp"
#include "y/utest/run.hpp"
#include "y/ios/ocstream.hpp"
#include "y/math/fit/sample/io.hpp"
#include "y/math/fit/ls.hpp"

using namespace upsylon;
using namespace math;

namespace
{
    class Shape : public ODE::ExplicitProblem<double>
    {
    public:
        explicit Shape() throw() {}
        virtual ~Shape() throw() {}
        
        virtual  size_t dimension() const throw()
        {
            return 2;
        }
        
        virtual void setup(array<double> &y, const array<double> &aorg, const Fit::Variables &vars) const throw()
        {
            const double phi = vars(aorg,"phi");
            y[1] = cos( start() + phi );
            y[2] = 0;
        }
        
        virtual void rates(array<double> &dydx,
                           double ,
                           const array<double>  &y,
                           const array<double>  &aorg,
                           const Fit::Variables &vars)
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
        
        virtual double query(const double,
                             const array<double>  &y,
                             const array<double>  &,
                             const Fit::Variables & ) const
        {
            return y[1];
        }
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(Shape);
        
    };
}

Y_UTEST(lsf_ode)
{
    // create a shape that implements the ExplicitProblem
    Shape                                      shape;

    {
        const ODE::ExplicitSolver<double>::Pointer driver0 = ODE::DriverCK<double>::New();
        ODE::ExplODE<double>                       F0(shape,driver0);
        F0->eps = 1e-3;
    }


    // create the associated Explicit ODE
    ODE::ExplODE<double>  F(shape);
    F->eps = 1e-3;


    const size_t NP    = 50 + alea.leq(50);
	const double range = double(5 + alea.leq(10));
    double       noise = 0.1;

    // create the sample
    vector<double> X,Y,Yf;
    Fit::Sample<double> sample(X,Y,Yf);
    
    Fit::Variables &vars = sample.variables; vars << "omega" << "lambda" << "phi";;
    const size_t    nvar = vars.size();
    std::cerr << "vars=" << vars << std::endl;
    vector<double> aorg(nvar,0);
    vector<double> aerr(nvar,0);
    vector<bool>   used(nvar,0);
    
    double &omega  = vars(aorg,"omega");
    double &lambda = vars(aorg,"lambda");
    double &phi    = vars(aorg,"phi");

    omega  = 8.0;
    lambda = 0.3;
    phi    = 0.8;

    for(size_t i=NP;i>0;--i)
    {
        X.push_back( range * alea.to<double>() );
        Y.push_back( cos( omega * X.back() + phi ) * exp(-lambda*X.back()) + noise*alea.symm<double>()) ;
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
            vars.display(std::cerr,used);
            std::cerr << "Couldn't fit..." << std::endl;
            return 0;
        }
    }
    vars.display(std::cerr, aorg, aerr);
    Fit::IO::Save("ode-fit1.dat",sample,true);

    vars.on(used, "lambda");
    {
        if( !ls.fit(sample, F, aorg, aerr, used) )
        {
            vars.display(std::cerr,used);
            std::cerr << "Couldn't fit..." << std::endl;
            return 0;
        }
    }

    vars.display(std::cerr, aorg, aerr);
    Fit::IO::Save("ode-fit2.dat",sample,true);

    if(false)
    {
        vars.on(used, "phi");
        {
            if( !ls.fit(sample, F, aorg, aerr, used) )
            {
                vars.display(std::cerr,used);
                std::cerr << "Couldn't fit..." << std::endl;
                return 0;
            }
        }

        vars.display(std::cerr, aorg, aerr);
        Fit::IO::Save("ode-fit3.dat",sample,true);
    }


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

