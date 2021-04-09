
#include "y/yap/natural.hpp"
 

namespace upsylon
{
    namespace yap
    {
        natural natural:: mul(const word_type *lhs, const size_t lnw, const word_type *rhs, const size_t rnw)
        {
            if(lnw>0&&rnw>0)
            {
                return fmul(lhs,lnw,rhs,rnw);
            }
            else
            {
                return natural();
            }
        }
    }
    
}

