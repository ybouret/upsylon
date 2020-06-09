//! \file
#ifndef Y_MKL_ODE_EXPLICIT_STEP_INCLUDED
#define Y_MKL_ODE_EXPLICIT_STEP_INCLUDED 1

#include "y/mkl/ode/types.hpp"

namespace upsylon {

    namespace mkl {
        
        namespace ODE {

            //! memory and method for one step
            template <typename T>
            class ExplicitStep : public Field<T>::Arrays
            {
            public:
                typedef typename Field<T>::Equation equation; //!< equation alias
                typedef typename Field<T>::Callback callback; //!< callback alias
                
                virtual ~ExplicitStep() throw();
                
                //! compute evaluation and embedded error
                /**
                 \param yout output
                 \param yerr embedded error
                 \param drvs the differential equation
                 \param x    starting point
                 \param h    guess step
                 \param y    initial values at x
                 \param dydx initial derivatives at x
                 \param cb   called BEFORE the equation on a trial point/time
                 The callback is used to modify (i.e. project or something)
                 the trial vectors.
                 */
                virtual  void operator()(array<T>        &yout,
                                         array<T>        &yerr,
                                         equation        &drvs,
                                         const T          x,
                                         const T          h,
                                         const array<T>  &y,
                                         const array<T>  &dydx,
                                         callback        *cb
                                         ) = 0;
                
                
            protected:
                explicit ExplicitStep(size_t na); //!< constructor
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(ExplicitStep);
                
            };
            
        }
    }
    
}

#endif
