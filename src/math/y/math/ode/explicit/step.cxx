#include "y/math/ztype.hpp"
#include "y/math/ode/explicit/step.hpp"


namespace upsylon
{
    namespace math
    {
        
        namespace ODE
        {
            
            
            template <> ExplicitStep<real_t>:: ~ExplicitStep() throw() {}
            
            template <>
            ExplicitStep<real_t> :: ExplicitStep( size_t na ) :
            Field<real_t>::Arrays(na)
            {
            }
            
            
            
        }
        
    }
    
}

