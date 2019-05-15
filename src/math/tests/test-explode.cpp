#include "y/math/ode/explicit/explode.hpp"
#include "y/math/ode/explicit/driver-ck.hpp"
#include "y/ios/ocstream.hpp"
#include "y/sequence/list.hpp"
#include "y/type/point2d.hpp"

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


        virtual double delta() const throw() { return 0.05; }

        virtual void compute( array<double> &dydx, double x, const array<double> &y )
        {
            dCos(dydx,x,y);
        }

        virtual double begin() const throw() { return 0; }
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(Something);
    };

    typedef point2d<double> Point;
    typedef list<Point>     Points;

    class Collector : public Points
    {
    public:

        inline  Collector() throw() {}
        inline ~Collector() throw() {}

        void Get( double x, const array<double> &Y )
        {
            const Point P(x,Y[1]);
            push_back(P);
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Collector);
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

    Collector     out;
    IODE::Collect com( & out, & Collector::Get );
    {
        std::cerr << "Collecting Linear" << std::endl;
        out.free();
        (void) iode.at(10, &com);
        ios::ocstream fp("iode_com_lin.dat");
        for( Collector::iterator p = out.begin(); p != out.end(); ++p )
        {
            fp("%g %g\n", p->x, p->y );
        }
    }

    {
        std::cerr << "Collecting Logarithmic" << std::endl;
        out.free();
        (void) iode.lnRun(1e-3,10, &com);
        ios::ocstream fp("iode_com_log.dat");
        for( Collector::iterator p = out.begin(); p != out.end(); ++p )
        {
            fp("%g %g\n", p->x, p->y );
        }
    }


}
Y_UTEST_DONE()


