//! \file
#ifndef Y_ODE_STIFF_DRIVER_RS_INCLUDED
#define Y_ODE_STIFF_DRIVER_RS_INCLUDED 1

#include "y/math/ode/implicit/solver.hpp"
#include "y/math/ode/implicit/shampine.hpp"

namespace upsylon
{
    namespace math 
    {
        namespace ODE
        {

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
