//! \file
#ifndef Y_MATH_MINIMIZE_INCLUDED
#define Y_MATH_MINIMIZE_INCLUDED 1

#include "y/math/triplet.hpp"

#include "y/math/types.hpp"
#include <iostream>

namespace upsylon
{
    namespace math
    {

        //! function to minimize
        struct minimize
        {
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

            template <typename T, typename FUNC> static inline
            T run(FUNC       &func,
                  triplet<T> &x,
                  triplet<T> &f)
            {
                assert(x.is_ordered());
                x.co_sort(f);
                assert(x.a<=x.b);
                assert(x.b<=x.c);
                assert(f.b<=f.a);
                assert(f.b<=f.c);

                T dx_prev = __fabs(x.c-x.a);
                for(;;)
                {
                    __step<T,FUNC>(func,x,f);
                    const T dx_curr = __fabs(x.c-x.a);
                    if(dx_curr>=dx_prev) break;
                    dx_prev = dx_curr;
                }
                return x.b;
            }

        };

    }

}


#endif

