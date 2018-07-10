//! \file
#ifndef Y_CODE_XBITREV_INCLUDED
#define Y_CODE_XBITREV_INCLUDED 1

#include "y/type/bswap.hpp"
#include "y/type/complex.hpp"

namespace upsylon
{
    struct xbitrev
    {
        //! reordering arr[1..size*2]
        template <typename T> static inline
        void run_safe( T arr[], const size_t size) throw()
        {
            const size_t n = size << 1;
            size_t       j = 1;
            for( size_t i=1; i<n; i+=2)
            {
                if(j>i)
                {
                    core::bswap<2*sizeof(T)>( &arr[i], &arr[j] );
                }
                size_t m = size;
                while( (m>=2) && (j>m) )
                {
                    j -=  m;
                    m >>= 1;
                }
                j += m;
            }
        }
    };

}

#endif

