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

        virtual void setup( array<double> &y  )
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

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Something);
    };

    class SomethingLin : public Something
    {
    public:
        explicit SomethingLin() throw() {}
        virtual ~SomethingLin() throw() {}

        virtual double begin() const throw() { return 0; }


    private:
        Y_DISABLE_COPY_AND_ASSIGN(SomethingLin);
    };

    class SomethingLog : public Something
    {
    public:
        explicit SomethingLog() throw() {}
        virtual ~SomethingLog() throw() {}

        virtual double begin() const throw() { return log(0.01); }


    private:
        Y_DISABLE_COPY_AND_ASSIGN(SomethingLog);
    };
    

}

Y_UTEST(explode)
{


    IODE::Solver  solver     = ODE::DriverCK<double>::New();
    IODE::Problem problemLin = new SomethingLin();
    IODE::Problem problemLog = new SomethingLog();

    IODE          iodeLin(solver,problemLin);
    IODE          iodeLog(solver,problemLog);

    ios::ocstream fp("iode.dat");

    for(double x=0.1;x<=10;x+=0.1)
    {
        fp("%g %g %g\n",x,iodeLin.at(x),iodeLog.at_log(log(x)));
    }



}
Y_UTEST_DONE()


