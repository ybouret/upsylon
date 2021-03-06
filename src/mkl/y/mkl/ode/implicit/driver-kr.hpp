//! \file
#ifndef Y_MKL_ODE_STIFF_DRIVER_KR_INCLUDED
#define Y_MKL_ODE_STIFF_DRIVER_KR_INCLUDED 1

#include "y/mkl/ode/implicit/solver.hpp"
#include "y/mkl/ode/implicit/kaps-rentrop.hpp"

namespace upsylon {

    namespace mkl {

        namespace ODE {
            
            //! Rosenbrock Kaps-Rentrop stiff integrator
            template <typename T>
            struct DriverKR
            {
                typedef ImplicitDriver<T,KapsRentrop> Type; //!< the integrator
            };
            
        }
        
    }
    
}


#endif
