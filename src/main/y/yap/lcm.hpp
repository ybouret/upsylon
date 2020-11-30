
//! \file

#ifndef YAP_LCM_INCLUDED
#define YAP_LCM_INCLUDED 1

#include "y/yap/natural.hpp"

namespace upsylon
{
    namespace yap
    {
        struct lcm
        {

            template <typename ITERATOR> static inline
            natural of(ITERATOR curr, size_t size)
            {
                if(size>0)
                {
                    natural res = *curr;
                    if(0==res) goto RETURN1;
                    ++curr;
                    --size;
                    while(size-- > 0 )
                    {
                        const natural tmp(*curr);
                        if(0==tmp) goto RETURN1;
                        ++curr;
                        res = apn::lcm(res,tmp);
                    }
                    return res;

                }
            RETURN1:
                return natural(1);

            }

            template <typename SEQUENCE> static inline
            natural of(SEQUENCE &seq)
            {
                return of( seq.begin(), seq.size() );
            }
        };
    }
}

#endif

