#ifndef YOCTO_ODE_IMPLICIT_STEP_INCLUDED
#define YOCTO_ODE_IMPLICIT_STEP_INCLUDED 1

#include "y/math/ode/types.hpp"

namespace upsylon
{
    namespace math
    {
        
        namespace ODE
        {
            
            template <typename T>
            class implicit_step : public Field<T>::Arrays
            {
            public:
                typedef typename Field<T>::Equation  equation;
                typedef typename Field<T>::Jacobian  jacobian;
                
                virtual ~implicit_step() throw();
                
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
                                        ) = 0;
            protected:
                explicit implicit_step( const size_t num );
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(implicit_step);
            };
            
        }
        
    }
}



#endif
