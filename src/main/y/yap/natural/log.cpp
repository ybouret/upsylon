
#include "y/yap/natural.hpp"
#include "y/exceptions.hpp"
#include "y/type/utils.hpp"

#include <cerrno>
#include <cmath>
#include <iostream>

namespace upsylon
{
    namespace yap
    {
        double natural:: log_of(const natural &n)
        {

            const size_t nbits = n.bits();
            if(nbits<=0)
            {
                throw libc::exception(EDOM,"natural::log_of(0)");
            }
            const size_t  p   = nbits-1;
            const natural den = exp2(p);
            assert(n>=den);
            assert(n<exp2(p+1));
            const double approx = log(2.0) * double(p) + log( ratio_of(n,den) );
            return approx;
        }

    }

}
