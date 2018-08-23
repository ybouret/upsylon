#ifndef YOCTO_ODE_STIFF_DRIVER_RS_INCLUDED
#define YOCTO_ODE_STIFF_DRIVER_RS_INCLUDED 1

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
            struct driverRS
            {
                typedef implicit_driver<T,shampine> type;
            };
            
        }
        
    }
    
}


#endif
