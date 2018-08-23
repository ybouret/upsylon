//! \file
#ifndef Y_MATH_ODE_EXPLICIT_RK45_INCLUDED
#define Y_MATH_ODE_EXPLICIT_RK45_INCLUDED 1

#include "y/math/ode/explicit/controler.hpp"

namespace upsylon
{
    namespace math
    {
        namespace ODE
        {
            //! Runge-Kutta 4/5 controller
            template <typename T>
			class RK45 : public ExplicitControler<T>
            {
            public:
                typedef typename Field<T>::Equation equation; //!< equation alias
                typedef typename Field<T>::Callback callback; //!< callback alias
                
                explicit RK45();
                virtual ~RK45() throw();
                
                //! try to forward
                /**
                 \param forward an explicit step (RKCK,RKDP)
                 \param y       input/output values
                 \param dydx    slope
                 \param drvs    the differential equation
                 \param x       input/output coordinate
                 \param h_try   trial step
                 \param h_did   final step
                 \param h_next  predicted net step
                 \param yscal   scaling values
                 \param eps     fractional tolerance for h
                 \param cb      a callback to correct trials
                 */
                void operator()(ExplicitStep<T>  &forward,
                                array<T>         &y,
                                const array<T>   &dydx,
                                equation         &drvs,
                                T                &x,
                                const T           h_try,
                                T                &h_did,
                                T                &h_next,
                                const array<T>   &yscal,
                                const T           eps,
                                callback         *cb);
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(RK45);
                array<T> &yerr,&ytmp;
            };
            
            
        }
    }
}

#endif

