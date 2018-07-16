
#include "y/mpl/integer.hpp"
#include "y/exceptions.hpp"
#include <cerrno>

namespace upsylon
{
    namespace mpl
    {
        const as_negative_t as_negative = {};

        integer integer::__div(const sign_type ls,
                               const uint8_t  *l,
                               const size_t    nl,
                               const sign_type rs,
                               const uint8_t  *r,
                               const size_t    nr)
        {
            switch(ls)
            {
                case __negative: switch(rs)
                {
                    case __negative: { const natural p = natural::__div(l,nl,r,nr); return integer(p);             }
                    case __zero:     goto DIV_BY_ZERO; 
                    case __positive: { const natural p = natural::__div(l,nl,r,nr);  return integer(p,as_negative); } 
                } /* FALLTHRU */

                case __zero:
                    return integer();

                case __positive: switch(rs)
                {
                    case __negative: { const natural p = natural::__div(l,nl,r,nr); return integer(p,as_negative); }
                    case __zero:    goto DIV_BY_ZERO;
                    case __positive: { const natural p = natural::__div(l,nl,r,nr); return integer(p);             }
                }
            }
        DIV_BY_ZERO:
            throw libc::exception(EDOM,"mpl.integer(division by zero)");
        }

    }
}


