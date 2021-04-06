//! \file
#ifndef Y_CODE_XBITREV_INCLUDED
#define Y_CODE_XBITREV_INCLUDED 1

#include "y/type/complex.hpp"

namespace upsylon
{
    //__________________________________________________________________________
    //
    //
    //! extended bit reversal algorithm
    //
    //__________________________________________________________________________
    struct xbitrev
    {

        //! C-style swapping two at once
        template <typename T>
        static inline void xswap2( T *a, T *b ) throw()
        {
            const T tmp0 = a[0];
            const T tmp1 = a[1];
            a[0] = b[0];
            a[1] = b[1];
            b[0] = tmp0;
            b[1] = tmp1;
        }

        //! swap pair of items
#define Y_XBITREV_SWAP(I,J) xbitrev::xswap2( &arr[I], &arr[J] )

#include "xbitrev-decl.hxx"
        //! reordering arr[1..size*2]
        template <typename T> static inline
        void run_( T arr[], const size_t size) throw()
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

        //! swap two pairs of items
#define Y_XBITREV_SWAP2(I,J) {  \
const size_t __I = (I);         \
const size_t __J = (J);         \
xswap2( &arr[__I], &arr[__J] ); \
xswap2( &brr[__I], &brr[__J] ); }

        //core::bswap<2*sizeof(T)>( &arr[__I], &arr[__J] );
        //core::bswap<2*sizeof(T)>( &brr[__I], &brr[__J] );



        //! reordering arr[1..size*2] and brr[1..size*2]
        template <typename T> static inline
        void run_( T arr[], T brr[], const size_t size) throw()
        {
            const size_t n = size << 1;
            size_t       j = 1;
            for( size_t i=1; i<n; i+=2)
            {
                if(j>i)
                {
                    Y_XBITREV_SWAP2(i,j)
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

        //! reordering arr[1..size*2] with generated swap tables
        template <typename T> static inline
        void run( T arr[], const size_t size) throw()
        {
            switch(size)
            {
#include "xbitrev-impl.hxx"
                default:
                    run_(arr,size);
            }
        }

        //! reordering arr[1..size*2] and brr[1..size*2] with generated swap tables
        template <typename T> static inline
        void run( T arr[], T brr[], const size_t size) throw()
        {
            switch(size)
            {
#include "xbitrev-imp2.hxx"
                default:
                    run_(arr,brr,size);
            }
        }
    };

    //__________________________________________________________________________
    //
    //
    //! hard-wired xbitrev
    //
    //__________________________________________________________________________
    template <typename T,const size_t size> struct xbitrev_for
    {
        //! generic call
        static void run(T arr[]) throw()
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
    };

#include "xbitrev-for.hxx"
    

}

#endif

