#include "y/math/ztype.hpp"
#include "y/math/ode/explicit/rk45.hpp"
#include "y/math/types.hpp"
#include "y/type/utils.hpp"
#include "y/exceptions.hpp"
#include <cerrno>

namespace upsylon
{
    namespace math
    {
        namespace ODE
        {
            template <> RK45<real_t>:: ~RK45() throw() {}
            
            template <> RK45<real_t>:: RK45() :
            ExplicitControler<real_t>(2),
            yerr( next() ),
            ytmp( next() )
            {
                
            }
            
            template <>
			void RK45<real_t>::operator()(ExplicitStep<real_t>  &forward,
										  array<real_t>         &y,
										  const array<real_t>   &dydx,
										  equation              &drvs,
										  real_t                &x,
										  const real_t           h_try,
										  real_t                &h_did,
										  real_t                &h_next,
										  const array<real_t>   &yscal,
										  const real_t           eps,
                                          callback              *cb
                                          )
			{
				static const real_t SAFETY = REAL(0.9);
				static const real_t PGROW  = REAL(-0.2);
				static const real_t PSHRNK = REAL(-0.25);
				//The value ERRCON equals (5/SAFETY) raised to the power (1/PGROW), see use below
				static const real_t ERRCON = REAL(pow)( REAL(5.0)/SAFETY, REAL(1.0)/PGROW);
				static const real_t DEC    = REAL(0.1);
				static const real_t INC    = REAL(5.0);
				
				
				//--------------------------------------------------------------
				// sanity check
				//--------------------------------------------------------------
				assert( size() >= y.size() );
				
				
				//--------------------------------------------------------------
				// initialize
				//--------------------------------------------------------------
				const size_t n        = y.size();
				real_t       h        = h_try;
				real_t       errmax   = REAL(0.0);
				const bool   positive = h >= REAL(0.0);
				
				
				
				for (;;) {
					//----------------------------------------------------------
					// embbeded step+error
					//----------------------------------------------------------
					forward( ytmp, yerr, drvs, x, h, y, dydx,cb );
					
					//----------------------------------------------------------
					// accuracy evaluation
					//----------------------------------------------------------
					errmax=0;
					for (size_t i=n; i>0; --i)
						errmax= max_of<real_t>(errmax,REAL(fabs)(yerr[i]/yscal[i]));
					errmax /= eps;
					if (errmax <= REAL(1.0) )
						break; // ==> success
					
					//----------------------------------------------------------
					// shall retry
					//----------------------------------------------------------
					const real_t htemp=SAFETY*h*REAL(pow)(errmax,PSHRNK);
					h=(positive ? max_of<real_t>(htemp,DEC*h) : min_of<real_t>(htemp,DEC*h));
					
					//----------------------------------------------------------
					// control overflow
					//----------------------------------------------------------
					const real_t xnew=x+h;
					if( (positive && xnew <= x) || (!positive && xnew >= x ) )
						throw libc::exception( ERANGE, "stepsize underflow in RK45");
				}
				
				//--------------------------------------------------------------
				// sucess: finalize
				//--------------------------------------------------------------
				x += (h_did=h);
				
				if (errmax > ERRCON)
					h_next=SAFETY*h*REAL(pow)(errmax,PGROW);
				else
					h_next=INC*h;
				
				for (size_t i=n;i>0;--i)
					y[i]=ytmp[i];
			}
            
            
        }
    }
}
