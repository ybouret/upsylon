//! \file
#ifndef Y_MATH_ODE_EXPLICIT_RKDP_INCLUDED
#define Y_MATH_ODE_EXPLICIT_RKDP_INCLUDED 1


#include "y/math/ode/explicit/step.hpp"

namespace upsylon
{
	namespace math
	{
		
		namespace ODE
		{
			
			//! Runge-Kutta Dormand-Prince 4/5 step
			template <typename T>
			class RKDP : public ExplicitStep<T>
			{
			public:
				typedef typename Field<T>::Equation equation; //!< equation alias
                typedef typename Field<T>::Callback callback; //!< callback alias
                
				explicit RKDP();         //!< constructor
				virtual ~RKDP() throw(); //!< destructor
				
                //! take a step
                virtual void operator()(array<T>        &yout,
                                        array<T>        &yerr,
                                        equation        &drvs,
                                        const T          x,
                                        const T          h,
                                        const array<T>  &y,
                                        const array<T>  &dydx,
                                        callback        *cb
                                        );
			private:
				Y_DISABLE_COPY_AND_ASSIGN(RKDP);
				array<T> &ytmp, &ak2, &ak3, &ak4, &ak5, &ak6, &ak7;
			};
		}
		
	}
	
}

#endif
