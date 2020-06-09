#include "y/mkl/ztype.hpp"
#include "y/mkl/ode/explicit/controler.hpp"

namespace upsylon
{
    namespace math
    {
        namespace ODE
        {
            
            template <>
            ExplicitControler<real_t>:: ~ExplicitControler() throw() {}
            
            template <>
            ExplicitControler<real_t>:: ExplicitControler( size_t na ) :
            Field<real_t>::Arrays(na)
            {
            }
            
        }
    }
}
