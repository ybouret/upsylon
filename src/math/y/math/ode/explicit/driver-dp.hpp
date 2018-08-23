//! \file
#ifndef Y_MATH_ODE_EXPLICIT_DRIVER_DP_INCLUDED
#define Y_MATH_ODE_EXPLICIT_DRIVER_DP_INCLUDED 1

#include "y/math/ode/explicit/solver.hpp"
#include "y/math/ode/explicit/rk45.hpp"
#include "y/math/ode/explicit/rkdp.hpp"

namespace upsylon
{
    namespace math
    {
        namespace ODE
        {
            //! Assemble a driver for Dormand-Prince
            template <typename T>
            struct DriverDP
            {
                typedef ExplicitDriver<T,RK45,RKDP> Type; //!< the integrator
            };
        }
    }
}
#endif
