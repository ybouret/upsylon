#include "y/mkl/ztype.hpp"
#include "y/mkl/ode/implicit/step.hpp"


namespace upsylon {

    namespace mkl {
        
        namespace ODE {

            template <> ImplicitStep<real_t>:: ~ImplicitStep() throw() {}
            
            template <> ImplicitStep<real_t>:: ImplicitStep(const size_t num) :
            Field<real_t>::Arrays(num)
            {}
            
        }
        
    }
}

