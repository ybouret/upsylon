
#include "y/yap/natural.hpp"


namespace upsylon
{
    namespace yap
    {

        natural natural::lmul(const word_type *lhs, const size_t lnw, const word_type *rhs, const size_t rnw)
        {
            if(lnw>0&&rnw>0)
            {
                const size_t pnw = lnw+rnw+1;
                return natural();
            }
            else
            {
                return natural();
            }
        }
    }

}
