//! \file
#ifndef Y_ODE_KAPS_RENTROP_INCLUDED
#define Y_ODE_KAPS_RENTROP_INCLUDED 1

#include "y/math/ode/implicit/rosenbrock.hpp"

namespace upsylon
{
    namespace math
    {
        
        namespace ODE
        {
            
            template <typename T>
            class KapsRentrop : public Rosenbrock<T>
            {
            public:
                virtual ~KapsRentrop() throw();
                explicit KapsRentrop();
                
                typedef typename ImplicitStep<T>::equation equation;
                typedef typename ImplicitStep<T>::jacobian jacobian;
                
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
