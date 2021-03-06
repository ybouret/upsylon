
#include "y/yap/natural.hpp"
 

namespace upsylon
{
    namespace yap
    {
        
        
        size_t natural:: switch_to_fft = 512;
        
        natural natural:: mul(const word_type *lhs, const size_t lnw, const word_type *rhs, const size_t rnw)
        {
            if(lnw>0&&rnw>0)
            {
                if(lnw>=switch_to_fft||rnw>=switch_to_fft)
                {
                    return fmul(lhs,lnw,rhs,rnw);
                }
                else
                {
                    return lmul(lhs,lnw,rhs,rnw);
                }
            }
            else
            {
                return natural();
            }
        }
    }
    
}

