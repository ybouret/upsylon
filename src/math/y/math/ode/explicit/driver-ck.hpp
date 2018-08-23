//! \fi;e
#ifndef Y_MATH_ODE_EXPLICIT_DRIVER_CK_INCLUDED
#define Y_MATH_ODE_EXPLICIT_DRIVER_CK_INCLUDED 1

#include "y/math/ode/explicit/solver.hpp"
#include "y/math/ode/explicit/rk45.hpp"
#include "y/math/ode/explicit/rkck.hpp"

namespace upsylon
{
    namespace math
    {
        namespace ODE
        {
            template <typename T>
            struct driverCK
            {
                typedef explicit_driver<T,RK45,RKCK> type;
            };
        }
    }
}
#endif
