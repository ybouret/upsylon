#include "y/math/ode/explicit/explode.hpp"
#include "y/math/ode/explicit/driver-ck.hpp"
#include "y/ios/ocstream.hpp"

#include "y/utest/run.hpp"

using namespace upsylon;
using namespace math;


namespace
{
    typedef ODE::ExplODE<double> IODE;

    class Something : public IODE::ProblemType
    {
    public:
        explicit Something() : a(1.2) {}
        virtual ~Something() throw() {}

        double a;

        virtual size_t dimension() const throw()
        {
            return 2;
        }

        void dCos( array<double> &dydx, const double, const array<double> &y ) throw()
        {
            dydx[1] = y[2];
            dydx[2] = - a * y[1];
        }

        virtual void setup( array<double> &y  ) const throw()
        {
            y[1] = 1;
            y[2] = 0;
        }

        virtual double query( const array<double> &y, double ) const
        {
            return y[1];
        }


        virtual double delta() const throw() { return 0.01; }

        virtual void compute( array<double> &dydx, double x, const array<double> &y )
        {
            dCos(dydx,x,y);
        }

        virtual double begin() const throw() { return 0; }
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(Something);
    };

    
    

}

Y_UTEST(explode)
{

    // get a solver
    IODE::Solver                    solver  = ODE::DriverCK<double>::New();

    // create a problem ( here using new Something() )
    IODE::Embedded<Something>::Type problem;

    // create the integrator
    IODE                            iode(solver,problem.pointer);

    // tune the solver if needed
    solver->eps = 1e-5;
    
    const double dx = 0.01;
    
    {
        std::cerr << "Compute Full" << std::endl;
        ios::ocstream fp("iode_full.dat");
        for(double x=problem->begin();x<=10;x+=dx)
        {
            const array<double> &Y = iode.at(x);
            fp("%g %g\n",x,Y[1]);
        }
    }

    {
        std::cerr << "Compute Full/Log" << std::endl;
        ios::ocstream fp("iode_flog.dat");
        for(double x=dx;x<=10;x+=dx)
        {
            const array<double> &Y = iode.lnRun(dx,x);
            fp("%g %g\n",x,Y[1]);
        }
    }

    
    {
        std::cerr << "Compute Fast" << std::endl;
        ios::ocstream fp("iode_fast.dat");
        const array<double> &Y = iode.reset();
        fp("%g %g\n", iode.current(), Y[1]);
        for(double x=iode.current()+dx;x<=10;x+=dx)
        {
            (void)iode.update(x);
            fp("%g %g\n", iode.current(), Y[1]);
        }

    }



}
Y_UTEST_DONE()


