//! \file
#ifndef Y_MKL_MINIMIZE_INCLUDED
#define Y_MKL_MINIMIZE_INCLUDED 1

#include "y/mkl/opt/bracket.hpp"
#include "y/mkl/types.hpp"
#include <iostream>

namespace upsylon {

    namespace mkl {

        //! function to minimize
        struct minimize
        {
            //! pre-processing of input
            enum bracketing
            {
                direct, //!< assume already bracketed
                inside, //!< bracket::inside before call
                expand  //!< bracket::expand before call
            };

            //! 1 step minimize an ordered x triplet with associated values
            template <typename T, typename FUNC> static inline
            void __step(FUNC       &func,
                        triplet<T> &x,
                        triplet<T> &f)
            {
                static const T C = T(0.381966011250105);
                assert(x.a<=x.b);
                assert(x.b<=x.c);
                assert(f.b<=f.a);
                assert(f.b<=f.c);
                const T ab = max_of<T>(x.b - x.a,0);
                const T bc = max_of<T>(x.c - x.b,0);
                if(bc>ab)
                {
                    //__________________________________________________________
                    //
                    // probe at right
                    //__________________________________________________________
                    const T xu = clamp(x.b,x.b+C*bc,x.c);
                    const T fu = func(xu);
                    if(fu>=f.b)
                    {
                        // move c->u
                        x.c = xu;
                        f.c = fu;
                    }
                    else
                    {
                        // move a->b, b->u
                        x.a = x.b;
                        f.a = f.b;
                        x.b = xu;
                        f.b = fu;
                    }
                }
                else
                {
                    //__________________________________________________________
                    //
                    // probe at left
                    //__________________________________________________________
                    const T xu = clamp(x.a,x.b-C*ab,x.b);
                    const T fu = func(xu);
                    if(fu>=f.b)
                    {
                        // move a->u
                        x.a = xu;
                        f.a = fu;
                    }
                    else
                    {
                        // move c->b, b->u
                        x.c = x.b;
                        f.c = f.b;
                        x.b = xu;
                        f.b = fu;
                    }
                }
                x.co_sort(f);
            }

            //! run minimisation steps until convergence
            /**
             \param func a 1d function
             \param x    initial variables
             \param f    initial f(x)
             \param how  pre-processing:
             - direct: x,f are ready
             - inside: bracket inside (x.a,x.c)
             - expand: bracket expand from (x.a,x.b)
             \param xtol stop at xtol
             */
            template <typename T, typename FUNC> static inline
            T run(FUNC            &func,
                  triplet<T>      &x,
                  triplet<T>      &f,
                  const bracketing how  = direct,
                  T                xtol = 0
                  )
            {
                //______________________________________________________________
                //
                // constants
                //______________________________________________________________
                static const T ftol     = numeric<T>::ftol;
                static const T xtol_min = numeric<T>::sqrt_ftol;
                xtol = max_of( fabs_of(xtol), xtol_min );

                //______________________________________________________________
                //
                // pre-process ?
                //______________________________________________________________
                switch(how)
                {
                    case direct: break;
                    case inside: bracket::inside(func,x,f); break;
                    case expand: bracket::expand(func,x,f); break;
                }

                //______________________________________________________________
                //
                // setup triplets
                //______________________________________________________________
                assert(x.is_ordered());
                x.co_sort(f); assert(x.a<=x.b); assert(x.b<=x.c); assert(f.b<=f.a); assert(f.b<=f.c);


                T dx_prev = fabs_of(x.c-x.a);
                for(;;)
                {
                    //__________________________________________________________
                    //
                    // reduce interval
                    //__________________________________________________________
                    __step<T,FUNC>(func,x,f);
                    assert(x.a<=x.b);
                    assert(x.b<=x.c);
                    assert(f.b<=f.a);
                    assert(f.b<=f.c);

                    //__________________________________________________________
                    //
                    // check convergence on values
                    //__________________________________________________________
                    {
                        const T df = max_of(fabs_of(f.c-f.b),fabs_of(f.a-f.b));
                        const T ff = ftol * fabs_of(f.b);
                        if(df<=ff) break;
                    }

                    //__________________________________________________________
                    //
                    // check convergence on interval
                    //__________________________________________________________
                    {
                        const T dx = fabs_of(x.c-x.a);
                        const T xx = xtol * fabs_of(x.b);
                        if(dx<=xx||dx>=dx_prev) break;
                        dx_prev = dx;
                    }
                }
                return x.b;
            }




        };

    }

}


#endif

