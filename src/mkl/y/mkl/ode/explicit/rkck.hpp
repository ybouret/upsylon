//! \file
#ifndef Y_MKL_ODE_RKCK_INCLUDED
#define Y_MKL_ODE_RKCK_INCLUDED 1


#include "y/mkl/ode/explicit/step.hpp"

namespace upsylon {

	namespace mkl {
		
		namespace ODE {
            
            
            //! Runge-Kutta Cash-Karp 4/5 step
            template <typename T>
            class RKCK : public ExplicitStep<T>
            {
            public:
                typedef typename Field<T>::Equation equation; //!< equation alias
                typedef typename Field<T>::Callback callback; //!< callback alias
                
                explicit RKCK();         //!< constructor
                virtual ~RKCK() throw(); //!< destructor
                
                //! take the step
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
                Y_DISABLE_COPY_AND_ASSIGN(RKCK);
                array<T> &ytmp, &ak2, &ak3, &ak4, &ak5, &ak6;
            };
            
		}
		
	}
	
}

#endif

