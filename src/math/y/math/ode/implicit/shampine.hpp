//! \file
#ifndef Y_ODE_SHAMPINE_INCLUDED
#define Y_ODE_SHAMPINE_INCLUDED 1

#include "y/math/ode/implicit/rosenbrock.hpp"

namespace upsylon
{
    namespace math
    {
        
        namespace ODE
        {
            
            template <typename T>
            class Shampine : public Rosenbrock<T>
            {
            public:
                virtual ~Shampine() throw();
                explicit Shampine();
                
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
                Y_DISABLE_COPY_AND_ASSIGN(Shampine);
            };

        }
        
    }
    
}


#endif
