//! \file
#ifndef Y_MKL_ODE_KAPS_RENTROP_INCLUDED
#define Y_MKL_ODE_KAPS_RENTROP_INCLUDED 1

#include "y/mkl/ode/implicit/rosenbrock.hpp"

namespace upsylon {

    namespace math {
        
        namespace ODE {
            
            //! Kaps-Rentrop method
            template <typename T>
            class KapsRentrop : public Rosenbrock<T>
            {
            public:
                virtual ~KapsRentrop() throw(); //!< destructor
                explicit KapsRentrop();         //!< constructor
                
                typedef typename ImplicitStep<T>::equation equation; //!< equation alias
                typedef typename ImplicitStep<T>::jacobian jacobian; //!< jacobian alias
                
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
                Y_DISABLE_COPY_AND_ASSIGN(KapsRentrop);
            };
            
        }
        
    }
    
}


#endif
