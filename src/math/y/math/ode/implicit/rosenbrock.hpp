//! \file
#ifndef YOCTO_ODE_ROSENBROCK_INCLUDED
#define YOCTO_ODE_ROSENBROCK_INCLUDED 1

#include "y/math/ode/implicit/step.hpp"
#include "y/math/kernel/lu.hpp"

namespace upsylon
{
    namespace math
    {
        namespace ODE
        {
            
            template <typename T>
            class Rosenbrock : public ImplicitStep<T>
            {
            public:
                virtual ~Rosenbrock() throw();
                void     prepare( size_t nv );
                
                
            protected:
                explicit Rosenbrock();
                array<T>    &g1, &g2, &g3, &g4, &dfdx,&err,&ysav,&dysav;
                matrix<T>    dfdy;
                matrix<T>    a;
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Rosenbrock);
            };

                     
        }
        
    }
    
}



#endif
