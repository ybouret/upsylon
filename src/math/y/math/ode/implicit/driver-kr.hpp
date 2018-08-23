//! \file
#ifndef Y_ODE_STIFF_DRIVER_KR_INCLUDED
#define Y_ODE_STIFF_DRIVER_KR_INCLUDED 1

#include "y/math/ode/implicit/solver.hpp"
#include "y/math/ode/implicit/kaps-rentrop.hpp"

namespace upsylon
{
    namespace math 
    {
        namespace ODE
        {
            
            //! Rosenbrock Kaps-Rentrop stiff integrator
            template <typename T>
            struct DriverKR
            {
                typedef ImplicitDriver<T,KapsRentrop> Type;
            };
            
        }
        
    }
    
}


#endif
