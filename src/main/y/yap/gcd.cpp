
#include "y/yap/gcd.hpp"

namespace upsylon
{
    namespace yap
    {

        natural compute_gcd::from(const natural &lhs, const natural &rhs)
        {
            if(lhs<=0||rhs<=0)
            {
                return 1;
            }
            else
            {
                return natural::gcd(lhs,rhs);
            }
        }

        integer compute_gcd::from(const integer &lhs, const integer &rhs)
        {
            const natural n = from(lhs.n,rhs.n);
            return integer(__positive,n);
        }
    }
}

