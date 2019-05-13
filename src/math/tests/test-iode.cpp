#include "y/math/ode/explicit/iode.hpp"
#include "y/math/ode/explicit/driver-ck.hpp"

#include "y/utest/run.hpp"

using namespace upsylon;
using namespace math;


namespace
{
    typedef ODE::ExplicitIODE<double> IODE;

    class Something : public IODE::ProblemType
    {
    public:
        explicit Something() : a(0.1) {}
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

        virtual void setup( array<double> &y  )
        {
            y[1] = 1;
            y[2] = 0;
        }

        virtual double finalize( const array<double> &y, double ) const
        {
            return y[1];
        }

        virtual double init() const throw() { return 0; }

        virtual double safe() const throw() { return 0.01; }

        virtual void compute( array<double> &dydx, double x, const array<double> &y )
        {
            dCos(dydx,x,y);
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Something);
    };

    

}

Y_UTEST(iode)
{


    IODE::Solver  solver  = ODE::DriverCK<double>::New();
    IODE::Problem problem = new Something();

    IODE iode(solver,problem);

    iode.Linear(1);

}
Y_UTEST_DONE()


