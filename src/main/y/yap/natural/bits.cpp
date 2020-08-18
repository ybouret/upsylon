
#include "y/yap/natural.hpp"
#include "y/code/round.hpp"

namespace upsylon
{
    namespace yap
    {
        natural natural::exp2(const size_t nbits)
        {
            const size_t ibit = nbits+1;
            const size_t imax = Y_ROUND8(ibit)>>3;
            const size_t itop = imax-1;
            natural      ans(imax,as_capacity);
            ans.get(itop) = bits_table::value[nbits&7];
            ans.bytes = imax;
            ans.update();
            assert(ans.bits()==ibit);
            return  ans;
        }
    }
}

