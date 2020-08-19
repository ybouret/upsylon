
#include "y/yap/natural.hpp"
#include "y/exceptions.hpp"
#include <cerrno>

namespace upsylon
{

    namespace yap
    {

        natural natural:: factorial(size_t n)
        {
            natural ans = 1;
            while(n>1)
            {
                ans *= n;
                --n;
            }
            return ans;
        }

        natural natural:: comb(const size_t n, const size_t k)
        {
            if(k>n)
            {
                throw libc::exception(EDOM,"natural::comb(k=%lu>n=%lu)",(unsigned long)k, (unsigned long)n);
            }
            natural ans = factorial(n);
            {
                const natural fk = factorial(k);
                ans /= fk;
            }
            {
                const natural fnmk = factorial(n-k);
                ans /= fnmk;
            }
            return ans;
        }

    }
}
