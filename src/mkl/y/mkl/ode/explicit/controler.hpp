//! \file
#ifndef Y_MKL_ODE_EXPLICIT_CONTROLER_INCLUDED
#define Y_MKL_ODE_EXPLICIT_CONTROLER_INCLUDED 1

#include "y/mkl/ode/explicit/step.hpp"

namespace upsylon {

    namespace mkl {

        namespace ODE {

            //! adaptive controler
            /**
             needs to be prepared before integration
             */
            template <typename T>
            class ExplicitControler : public Field<T>::Arrays
            {
            public:
                typedef typename Field<T>::Equation equation; //!< equation alias
                typedef typename Field<T>::Callback callback; //!< callback alias
                
                virtual ~ExplicitControler() throw(); //!< destructor
                
                //! try to forward
                /**
                 \param forward an explicit step (RKCK,RKDP)
                 \param y       input/output values
                 \param dydx    the slope
                 \param drvs    the differential equation
                 \param x       input/output coordinate
                 \param h_try   trial step
                 \param h_did   final step
                 \param h_next  predicted net step
                 \param yscal   scaling values
                 \param eps     fractional tolerance for h
                 \param cb      a callback to corect trials
                 */
                virtual void operator()(ExplicitStep<T>  &forward,
                                        array<T>         &y,
                                        const array<T>   &dydx,
                                        equation         &drvs,
                                        T                &x,
                                        const T           h_try,
                                        T                &h_did,
                                        T                &h_next,
                                        const array<T>   &yscal,
                                        const T           eps,
                                        callback         *cb) = 0;
            protected:
                explicit ExplicitControler(size_t na); //!< constructor
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(ExplicitControler);
            };
        }
    }
}

#endif
