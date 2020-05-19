//! \file

#ifndef Y_SPADE_FIELD_OPS_INCLUDED
#define Y_SPADE_FIELD_OPS_INCLUDED 1

#include "y/spade/fields.hpp"

namespace upsylon {
 
    namespace Spade
    {
     
        struct Ops
        {
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
        };
        
        
    }
    
}


#endif

