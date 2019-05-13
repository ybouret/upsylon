//! \file
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
            //! Assemble a driver for Cash-Karp
            template <typename T>
            struct DriverCK
            {
                typedef ExplicitDriver<T,RK45,RKCK> Type; //!< the integrator
                //! create a new solver
                static inline
                ExplicitSolver<T> *New()
                {
                    return new Type();
                }
            };
        }
    }
}
#endif
