//! \file
#ifndef Y_MKL_ODE_STIFF_DRIVER_RS_INCLUDED
#define Y_MKL_ODE_STIFF_DRIVER_RS_INCLUDED 1

#include "y/mkl/ode/implicit/solver.hpp"
#include "y/mkl/ode/implicit/shampine.hpp"

namespace upsylon {

    namespace mkl {

        namespace ODE {

            //! Rosenbrock Shampine stiff integrator
            template <typename T>
            struct DriverRS
            {
                typedef ImplicitDriver<T,Shampine> Type; //!< the integrator
            };
            
        }
        
    }
    
}


#endif
