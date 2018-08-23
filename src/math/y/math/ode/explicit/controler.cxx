#include "y/math/ztype.hpp"
#include "y/math/ode/explicit/controler.hpp"

namespace upsylon
{
    namespace math
    {
        namespace ODE
        {
            
            template <>
            explicit_controler<real_t>:: ~explicit_controler() throw() {}
            
            template <>
            explicit_controler<real_t>:: explicit_controler( size_t na ) :
            Field<real_t>::Arrays(na)
            {
            }
            
        }
    }
}
