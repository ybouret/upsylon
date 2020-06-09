//! \file
#ifndef Y_MKL_ODE_EXPLICIT_DRIVER_DP_INCLUDED
#define Y_MKL_ODE_EXPLICIT_DRIVER_DP_INCLUDED 1

#include "y/mkl/ode/explicit/solver.hpp"
#include "y/mkl/ode/explicit/rk45.hpp"
#include "y/mkl/ode/explicit/rkdp.hpp"

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
