//! \file
#ifndef Y_CODE_XBITREV_INCLUDED
#define Y_CODE_XBITREV_INCLUDED 1

#include "y/type/bswap.hpp"
#include "y/type/complex.hpp"

namespace upsylon
{
    //! extended bit reversal algorithm
    struct xbitrev
    {
#define Y_XBITREV_SWAP(I,J) core::bswap<2*sizeof(T)>( &arr[I], &arr[J] );
#include "xbitrev-decl.hxx"
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
                    Y_XBITREV_SWAP(i,j);
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

        template <typename T> static inline
        void run( T arr[], const size_t size) throw()
        {
            switch(size)
            {
#include "xbitrev-impl.hxx"

                default:
                    run_safe(arr,size);
            }
        }
    };

}

#endif

