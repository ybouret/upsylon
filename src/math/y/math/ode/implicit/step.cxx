#include "y/math/ztype.hpp"
#include "y/math/ode/implicit/step.hpp"


namespace upsylon
{
    namespace math 
    {
        
        namespace ODE
        {
            template <> ImplicitStep<real_t>:: ~ImplicitStep() throw() {}
            
            template <> ImplicitStep<real_t>:: ImplicitStep(const size_t num) :
            Field<real_t>::Arrays(num)
            {}
        }
        
    }
}

