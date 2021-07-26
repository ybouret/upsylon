//!\file

#ifndef Y_CORE_ISQRT_INCLUDED
#define Y_CORE_ISQRT_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon {

    //! integer square root
    struct isqrt
    {
        //! integer square root, raw method
        template <typename T> static inline
        T of(const T n) throw()
        {
            if(n<=1)
            {
                return n;
            }
            else
            {
                T x1 = (n>>1);
                while(true)
                {
                    const T x0 = x1;
                    x1 = ((x0+n/x0)>>1);
                    if(x1>=x0)
                    {
                        return x0;
                    }
                }
            }
        }

    };

    namespace mkl
    {
        //! dedicated sqrt for mkl functions
        size_t sqrt_of(const size_t) throw();
    }





}

#endif

