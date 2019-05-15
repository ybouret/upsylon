//! \file
#ifndef Y_MATH_ODE_IMPL_LSSI_INCLUDED
#define Y_MATH_ODE_IMPL_LSSI_INCLUDED

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

                template <typename T>
                static inline
                size_t ComputeSteps(const T sgn_width,
                                    T      &delta)
                {
                    const T abs_width = fabs_of(sgn_width);
                    if(abs_width<=0)
                    {
                        delta = 0;
                        return 0;
                    }
                    else
                    {
                        const T      abs_delta = fabs_of(delta);
                        assert(abs_delta>0);
                        if(abs_delta>=abs_width)
                        {
                            delta = sgn_width;
                            return 1;
                        }
                        else
                        {
                            assert(abs_delta<abs_width);
                            const size_t num_steps = static_cast<size_t>( ceil_of(abs_width/abs_delta) );
                            delta = sgn_width/num_steps;
                            return num_steps;
                        }
                    }

                }

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
                               T                            dx,
                               typename Field<T>::Callback *cb,
                               typename Field<T>::Collect  *com=0)
                {

                    //__________________________________________________________
                    //
                    // adjust number of steps
                    //__________________________________________________________
                    const T      sw = x2-x1;
                    const size_t ns = ComputeSteps<T>(sw,dx);

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
                        const T xnext = x1 + (i*sw)/ns;
                        driver(eqdiff,ystart,xcurr,xnext,ctrl,cb);
                        xcurr = xnext;
                        if(com) (*com)(xcurr,ystart);
                    }
                    
                    //__________________________________________________________
                    //
                    // final step
                    //__________________________________________________________
                    driver(eqdiff,ystart,xcurr,x2,ctrl,cb);
                    if(com) (*com)(x2,ystart);
                    
                }
                
                //! integrate from 0 to exp(lnMax), first step is 0->exp(lnMin), and not greater step than lnStep
                template <
                typename T,
                typename DRIVER
                > static inline
                void LogarithmicRun(DRIVER                      &driver,
                                    typename Field<T>::Equation &eqdiff,
                                    array<T>                    &ystart,
                                    const T                      lnMin,
                                    const T                      lnMax,
                                    T                            lnStep,
                                    typename Field<T>::Callback  *cb,
                                    typename Field<T>::Collect   *com=0)
                {
                    //__________________________________________________________
                    //
                    // compute internal steps
                    //__________________________________________________________
                    const T      sw = lnMax-lnMin;
                    const size_t ns = ComputeSteps<T>(sw,lnStep);
                    
                    //__________________________________________________________
                    //
                    // first step
                    //__________________________________________________________
                    T xcurr = exp( lnMin );
                    T ctrl  = xcurr/2;
                    driver(eqdiff,ystart,0,xcurr,ctrl,cb);
                    if(com) (*com)(xcurr,ystart);

                    //__________________________________________________________
                    //
                    // internal steps
                    //__________________________________________________________
                    ctrl = lnStep/2;
                    for(size_t i=1;i<ns;++i)
                    {
                        const T lnext = lnMin + (i*sw)/ns;
                        const T xnext = exp_of(lnext);
                        driver(eqdiff,ystart,xcurr,xnext,ctrl,cb);
                        xcurr = xnext;
                        if(com) (*com)(xcurr,ystart);
                    }
                    
                    //__________________________________________________________
                    //
                    //final step
                    //__________________________________________________________
                    const T xmax = exp_of(lnMax);
                    driver(eqdiff,ystart,xcurr,xmax,ctrl,cb);
                    if(com) (*com)(xmax,ystart);
                }
                
            };
            
        }
        
    }
    
}


#endif

