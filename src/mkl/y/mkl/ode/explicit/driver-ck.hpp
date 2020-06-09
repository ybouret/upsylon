//! \file
#ifndef Y_MKL_ODE_EXPLICIT_DRIVER_CK_INCLUDED
#define Y_MKL_ODE_EXPLICIT_DRIVER_CK_INCLUDED 1

#include "y/mkl/ode/explicit/solver.hpp"
#include "y/mkl/ode/explicit/rk45.hpp"
#include "y/mkl/ode/explicit/rkck.hpp"

namespace upsylon {

    namespace mkl {

        namespace ODE {

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
