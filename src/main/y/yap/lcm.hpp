
//! \file

#ifndef YAP_LCM_INCLUDED
#define YAP_LCM_INCLUDED 1

#include "y/yap/rational.hpp"

namespace upsylon
{
    namespace yap
    {
        //______________________________________________________________________
        //
        //
        //! computing least common multiple
        //
        //______________________________________________________________________
        struct lcm
        {

            //! compute for a range
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

            //! compute for a sequence
            template <typename SEQUENCE> static inline
            natural of(SEQUENCE &seq)
            {
                return of( seq.begin(), seq.size() );
            }

            //! lcm of a range of rational denominators
            template <typename ITERATOR> static inline
            integer of_denominators(ITERATOR curr, size_t size)
            {
                natural res(1);
                while(size-- > 0)
                {
                    const rational &q   = *(curr++);
                    const natural  &den = q.den; assert(den>0);
                    res = apn::lcm(res,den);
                }
                return res;
            }

            //! compute for a sequence of denominators
            template <typename SEQUENCE> static inline
            integer of_denominators(SEQUENCE &seq)
            {
                return of_denominators( seq.begin(), seq.size() );
            }

        };
    }
}

#endif

