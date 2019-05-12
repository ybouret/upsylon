//! \file
#ifndef Y_MATH_ODE_IMPL_LSS_INTEGRATOR_INCLUDED
#define Y_MATH_ODE_IMPL_LSS_INTEGRATOR_INCLUDED

#include "y/math/ode/explicit/solver.hpp"

namespace upsylon
{
    namespace math
    {
        namespace ODE
        {
            //! Limited Step Size Integrator
            struct LimitedStepSizeIntegrator
            {
                template <
                typename T,
                typename DRIVER
                > static inline
                void RunWith(DRIVER                      &driver,
                             typename Field<T>::Equation &eqdiff,
                             array<T>   &ystart,
                             const T     x1,
                             const T     x2,
                             const T     dx_safe,
                             typename Field<T>::Callback  *cb)
                {
                    assert(fabs_of(dx_safe)>0);

                    //__________________________________________________________
                    //
                    // prepare/update memory
                    //__________________________________________________________
                    driver.start(ystart.size());

                    //__________________________________________________________
                    //
                    // adjust number of steps
                    //__________________________________________________________
                    const T signed_length = x2-x1;
                    const T length        = fabs_of(signed_length);
                    const T abs_dx        = fabs_of(dx_safe);

                    size_t  ns            = 0;
                    T       dx            = 0;
                    if(length<=abs_dx)
                    {
                        ns     = 1;
                        dx     = signed_length;
                    }
                    else
                    {
                        assert(abs_dx<length);
                        ns     = static_cast<size_t>(length/abs_dx);
                        dx     = signed_length/ns;
                    }
                    //std::cerr << "nstep=" << ns << std::endl;
                    
                    //__________________________________________________________
                    //
                    // initialize
                    //__________________________________________________________
                    T ctrl  = dx/2;
                    T xcurr = x1;

                    //__________________________________________________________
                    //
                    // internal steps
                    //__________________________________________________________
                    for(size_t i=1;i<ns;++i)
                    {
                        const T xnext = x1 + (i*signed_length)/ns;
                        driver(eqdiff,ystart,xcurr,xnext,ctrl,cb);
                        xcurr = xnext;
                    }

                    //__________________________________________________________
                    //
                    // final step
                    //__________________________________________________________
                    driver(eqdiff,ystart,xcurr,x2,ctrl,cb);

                }
            };

        }

    }

}


#endif

