#include "y/mkl/ztype.hpp"
#include "y/mkl/ode/explicit/solver.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace math
    {
        namespace ODE
        {
            
            template <>
            ExplicitSolver<real_t>:: ~ExplicitSolver() throw() {}
            
            template <>
            ExplicitSolver<real_t>:: ExplicitSolver() :
            SolverData<real_t>()
            {
                
            }
            
            template <>
            void ExplicitSolver<real_t>:: step(Equation                   &drvs,
                                               ExplicitControler<real_t>  &ctrl,
                                               ExplicitStep<real_t>       &forward,
                                               array<real_t>              &ystart,
                                               const real_t                x1,
                                               const real_t                x2,
                                               real_t                      &h1,
                                               Callback                    *cb
                                               )
            {
                const size_t   n    = ystart.size();
                const real_t  _TINY = REAL(fabs)( TINY );
                
                //--------------------------------------------------------------
                // sanity check
                //--------------------------------------------------------------
                assert( size()         == n );
                assert( ctrl.size()    == n );
                assert( forward.size() == n );
                
                
                //--------------------------------------------------------------
                // initialize
                //--------------------------------------------------------------
                real_t h = x2 < x1 ? -fabs_of(h1) : fabs_of(h1);
                real_t x = x1;
                for( size_t i=n;i>0;--i) y[i] = ystart[i];
                
                //--------------------------------------------------------------
                // Main Loop
                //--------------------------------------------------------------
                for(;;)
                {
                    //----------------------------------------------------------
                    // initialize derivatives @x
                    //----------------------------------------------------------
                    drvs( dydx, x, y );
                    for( size_t i=n; i>0; --i )
                        yscal[i] = REAL(fabs)(y[i]) + REAL(fabs)( h * dydx[i] ) + _TINY;
                    
                    //----------------------------------------------------------
                    // check no overshooting
                    //----------------------------------------------------------
                    const real_t xm = x+h;
                    if( (xm-x2) * (xm-x1) >= 0 ) h = x2 - x;
                    
                    //----------------------------------------------------------
                    // forward with control
                    //----------------------------------------------------------
                    real_t h_did = 0, h_next = 0;
                    ctrl( forward, y, dydx, drvs, x, h, h_did, h_next, yscal, eps,cb);
                    
#if 0
                    if( Fabs(h_did) < Fabs(h) )
                    {
                        // had to reduce
                    }
#endif
                    
                    //----------------------------------------------------------
                    // check if done
                    //----------------------------------------------------------
                    if( (x-x1) * (x-x2) >= 0 )
                    {
                        break;
                    }
                    
                    //----------------------------------------------------------
                    // check if not too small
                    //----------------------------------------------------------
                    if( REAL(fabs)(h_next) < REAL(fabs)(hmin) )
                        throw exception( "h=%g<hmin=%g in ode::solver", h, hmin );
                    
                    h=h_next;
                }
                
                //--------------------------------------------------------------
                // success
                //--------------------------------------------------------------
                for( size_t i=n; i >0; --i )
                    ystart[i] = y[i];
                h1 = h;
                
            }
            
        }
    }
}
