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
        };
        
        
    }
    
}


#endif

