#include "y/utest/run.hpp"
#include "y/mkl/ode/implicit/driver-rs.hpp"
#include "y/mkl/ode/implicit/driver-kr.hpp"
#include "y/sequence/vector.hpp"

#include "y/ios/ocstream.hpp"

using namespace upsylon;
using namespace math;

typedef double Real;

namespace  {
    
    
    class problem
    {
    public:
        problem() {}
        ~problem() throw() {}
        
        void derivs( array<Real> &dydx, Real , const array<Real> &y )
        {
            dydx[1] = -0.013*y[1]-1000.0*y[1]*y[3];
            dydx[2] = -2500.0*y[2]*y[3];
            dydx[3] = -0.013*y[1]-1000.0*y[1]*y[3]-2500.0*y[2]*y[3];
        }
        
        
        void jacobn( array<Real> &dfdx, matrix<Real> &dfdy, Real , const array<Real> &y)
        {
            for (size_t i=1;i<=dfdx.size();i++) dfdx[i]=0.0; 
            dfdy[1][1] = -0.013-1000.0*y[3]; 
            dfdy[1][2] =  0.0;
            dfdy[1][3] = -1000.0*y[1]; 
            dfdy[2][1] =  0.0;
            dfdy[2][2] = -2500.0*y[3];
            dfdy[2][3] = -2500.0*y[2];
            dfdy[3][1] = -0.013-1000.0*y[3]; 
            dfdy[3][2] = -2500.0*y[3];
            dfdy[3][3] = -1000.0*y[1]-2500.0*y[2];
        }
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(problem);
    };
    
}



Y_UTEST(ode_impl)
{
    problem eq;
       
    ODE::Field<Real>::Equation derivs( &eq, & problem::derivs );
    ODE::Field<Real>::Jacobian jacobn( &eq, & problem::jacobn );
    
   
    vector<Real> y(3,0.0);
    
    
    Real dx = 0.01;
    
    {
        ODE::DriverRS<Real>::Type drv;
        drv.start(3);
        drv.eps = 1e-4;
        Real h  = 2.9e-4;
        y[1] = 1;
        y[2] = 1;
        y[3] = 0;
        
        ios::ocstream fp("stiff-rs.dat",false);
        fp("%g %g %g %g\n", 0.0, y[1], y[2], y[3] );
        for( size_t i=0; i < 1000; ++i )
        {
            Real x1 = i     * dx;
            Real x2 = (i+1) * dx;
            drv(derivs,jacobn,y,x1,x2,h);
            fp("%g %g %g %g\n", x2, y[1], y[2], y[3] );
        }
    }
    
    
    {
        ODE::DriverKR<Real>::Type drv;
        drv.start(3);
        drv.eps = 1e-3;
        Real h  = 2.9e-4;
        y[1] = 1;
        y[2] = 1;
        y[3] = 0;
        
        ios::ocstream fp("stiff-kr.dat",false);
        fp("%g %g %g %g\n", 0.0, y[1], y[2], y[3] );
        for( size_t i=0; i < 1000; ++i )
        {
            Real x1 = i     * dx;
            Real x2 = (i+1) * dx;
            drv(derivs,jacobn,y,x1,x2,h);
            fp("%g %g %g %g\n", x2, y[1], y[2], y[3] );
        }
    }

    
}
Y_UTEST_DONE()
