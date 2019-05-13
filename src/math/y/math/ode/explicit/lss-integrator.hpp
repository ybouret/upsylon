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
            struct LSSI
            {
                //! integrate linear step x1->x2, not faster than dx_safe
                template <
                typename T,
                typename DRIVER
                > static inline
                void LinearRun(DRIVER                      &driver,
                               typename Field<T>::Equation &eqdiff,
                               array<T>                    &ystart,
                               const T                      x1,
                               const T                      x2,
                               const T                      dx_safe,
                               typename Field<T>::Callback *cb)
                {
                    assert(fabs_of(dx_safe)>0);
                    
                    //__________________________________________________________
                    //
                    // prepare/update memory
                    //__________________________________________________________
                    //driver.start(ystart.size());
                    
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
                        ns     = static_cast<size_t>( ceil(length/abs_dx) );
                        dx     = signed_length/ns;
                    }
                    
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
                
                //! integrate from 0 to exp(lnMax), first step is 0->exp(lnMin), and not greater than lnStep
                template <
                typename T,
                typename DRIVER
                > static inline
                void LogarithmicRun(DRIVER                      &driver,
                                    typename Field<T>::Equation &eqdiff,
                                    array<T>                    &ystart,
                                    const T                      lnMin,
                                    const T                      lnMax,
                                    const T                      lnStep,
                                    typename Field<T>::Callback  *cb)
                {
                    //__________________________________________________________
                    //
                    // prepare/update memory
                    //__________________________________________________________
                    //driver.start(ystart.size());
                    
                    //__________________________________________________________
                    //
                    // compute internal steps
                    //__________________________________________________________
                    assert(lnMax>=lnMin);
                    assert(lnStep>0);
                    const T length  = lnMax-lnMin;
                    size_t  ns      = 0;
                    T       dl      = 0;
                    if(length<=lnStep)
                    {
                        ns = 1;
                        dl = length;
                    }
                    else
                    {
                        assert(lnStep<length);
                        ns     = static_cast<size_t>( ceil(length/lnStep) );
                        dl     = length/ns;
                    }
                    assert(ns>0);
                    
                    //__________________________________________________________
                    //
                    // first step
                    //__________________________________________________________
                    T xcurr = exp( lnMin );
                    T ctrl  = xcurr/2;
                    driver(eqdiff,ystart,0,xcurr,ctrl,cb);
                    
                    //__________________________________________________________
                    //
                    // internal steps
                    //__________________________________________________________
                    ctrl = dl/2;
                    for(size_t i=1;i<ns;++i)
                    {
                        const T lnext = lnMin + (i*length)/ns;
                        const T xnext = exp(lnext);
                        driver(eqdiff,ystart,xcurr,xnext,ctrl,cb);
                        xcurr = xnext;
                    }
                    
                    //__________________________________________________________
                    //
                    //final step
                    //__________________________________________________________
                    driver(eqdiff,ystart,xcurr,exp(lnMax),ctrl,cb);
                    
                }
                
            };
            
        }
        
    }
    
}


#endif

