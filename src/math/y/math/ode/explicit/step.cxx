#include "y/math/ztype.hpp"
#include "y/math/ode/explicit/step.hpp"


namespace upsylon
{
    namespace math
    {
        
        namespace ODE
        {
            
            
            template <> explicit_step<real_t>:: ~explicit_step() throw() {}
            
            template <>
            explicit_step<real_t> :: explicit_step( size_t na ) :
            Field<real_t>::Arrays(na)
            {
            }
            
            
            
        }
        
    }
    
}

