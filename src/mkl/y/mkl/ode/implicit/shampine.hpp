//! \file
#ifndef Y_MKL_ODE_SHAMPINE_INCLUDED
#define Y_MKL_ODE_SHAMPINE_INCLUDED 1

#include "y/mkl/ode/implicit/rosenbrock.hpp"

namespace upsylon {

    namespace mkl {
        
        namespace ODE {
            
            //! Shampine method
            template <typename T>
            class Shampine : public Rosenbrock<T>
            {
            public:
                virtual ~Shampine() throw(); //!< destructor
                explicit Shampine();         //!< constructor
                
                typedef typename ImplicitStep<T>::equation equation; //!< equation alias
                typedef typename ImplicitStep<T>::jacobian jacobian; //!< jacobian alias
                
                //! Shampine's step
                virtual void operator()(array<T>       &y,
                                        array<T>       &dydx,
                                        T              &x,
                                        T               htry,
                                        T               eps,
                                        array<T>       &yscal,
                                        T              &hdid,
                                        T              &hnext,
                                        equation       &derivs,
                                        jacobian       &jacobn
                                        );
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Shampine);
            };

        }
        
    }
    
}


#endif
