//! \file

#ifndef Y_SPADE_FIELD_OPS_INCLUDED
#define Y_SPADE_FIELD_OPS_INCLUDED 1

#include "y/spade/fields.hpp"

namespace upsylon {
 
    namespace Spade
    {

        //----------------------------------------------------------------------
        //
        //! operations on fields
        //
        //----------------------------------------------------------------------
        struct Ops
        {
            //! load the same value on a sub-layout
            template <typename FIELD> static inline
            void Ld(FIELD                           &F,
                    const typename FIELD::LayoutType sub,
                    typename       FIELD::param_type args)
            {
                assert(F.contains(sub));
                typename FIELD::LayoutType::Loop loop(sub.lower,sub.upper);
                for(loop.boot();loop.good();loop.next())
                {
                    F[loop.value] = args;
                }
            }

            //! count the number of same values
            template <typename FIELD> static inline
            size_t CountSame(const FIELD                      &F,
                             const typename FIELD::LayoutType sub,
                             typename       FIELD::param_type args)
            {
                assert(F.contains(sub));
                typename FIELD::LayoutType::Loop loop(sub.lower,sub.upper);
                size_t ans = 0;
                for(loop.boot();loop.good();loop.next())
                {
                    if(args==F[loop.value]) ++ans;
                }
                return ans;
            }

            //! count the number of different values
            template <typename FIELD> static inline
            size_t CountDiff(const FIELD                      &F,
                             const typename FIELD::LayoutType sub,
                             typename       FIELD::param_type args)
            {
                assert(F.contains(sub));
                typename FIELD::LayoutType::Loop loop(sub.lower,sub.upper);
                size_t ans = 0;
                for(loop.boot();loop.good();loop.next())
                {
                    if(args!=F[loop.value]) ++ans;
                }
                return ans;
            }

            //! Minimal value on a sub layout
            template <typename FIELD> static inline
            typename FIELD::mutable_type Min(const FIELD                      &F,
                                             const typename FIELD::LayoutType sub)
            {
                assert(F.contains(sub));
                typename FIELD::LayoutType::Loop loop(sub.lower,sub.upper); loop.boot();
                typename FIELD::mutable_type     vmin = F[*loop];
                for(loop.next();loop.good();loop.next())
                {
                    typename FIELD::const_type &temp = F[*loop];
                    if(temp<vmin) vmin = temp;
                }
                return vmin;
            }

            //! Maximal value on a sub layout
            template <typename FIELD> static inline
            typename FIELD::mutable_type Max(const FIELD                      &F,
                                             const typename FIELD::LayoutType sub)
            {
                assert(F.contains(sub));
                typename FIELD::LayoutType::Loop loop(sub.lower,sub.upper); loop.boot();
                typename FIELD::mutable_type     vmax = F[*loop];
                for(loop.next();loop.good();loop.next())
                {
                    typename FIELD::const_type &temp = F[*loop];
                    if(temp>vmax) vmax = temp;
                }
                return vmax;
            }

            //! Min/Max values on a sub layout
            template <typename FIELD> static inline
            void MinMax(typename FIELD::mutable_type     &vmin,
                        typename FIELD::mutable_type     &vmax,
                        const FIELD                      &F,
                        const typename FIELD::LayoutType sub)
            {
                assert(F.contains(sub));
                typename FIELD::LayoutType::Loop loop(sub.lower,sub.upper);
                loop.boot();
                vmax = vmin = F[*loop];
                for(loop.next();loop.good();loop.next())
                {
                    typename FIELD::const_type &temp = F[*loop];
                    if(temp>vmax)
                    {
                        vmax = temp;
                    }
                    else if(temp<vmin)
                    {
                        vmin = temp;
                    }

                }
            }

        };
        
        
    }
    
}


#endif

