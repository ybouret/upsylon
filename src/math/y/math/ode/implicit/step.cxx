#include "y/math/ztype.hpp"
#include "y/math/ode/implicit/step.hpp"


namespace upsylon
{
    namespace math 
    {
        
        namespace ODE
        {
            template <> implicit_step<real_t>:: ~implicit_step() throw() {}
            
            template <> implicit_step<real_t>:: implicit_step(const size_t num) :
            Field<real_t>::Arrays(num)
            {}
        }
        
    }
}

