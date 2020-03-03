//! \file
#ifndef Y_TYPE_BSWAP_INCLUDED
#define Y_TYPE_BSWAP_INCLUDED 1

#include "y/type/cswap.hpp"

namespace upsylon
{

    namespace core {

        //! byte swapping
        template <const size_t N> inline
        void bswap(void *a,void *b) throw()
        {
            uint8_t *A = (uint8_t *)a;
            uint8_t *B = (uint8_t *)b;
            for(size_t i=0;i<N;++i)
            {
                cswap(A[i],B[i]);
            }
        }

        //! special case: 0-length
        template <> inline void bswap<0>(void *,void*) throw() {}

#if 0
        //! special case: 1-length
        template <> inline void bswap<1>(void *a,void *b) throw()
        {
            cswap( *(uint8_t*)a, *(uint8_t*)b );
        }

        //! special case: 2-length
        template <> inline void bswap<2>(void *a,void *b) throw()
        {
            cswap( *(uint16_t*)a, *(uint16_t*)b );
        }

        //! special case: 4-length
        template <> inline void bswap<4>(void *a,void *b) throw()
        {
            cswap( *(uint32_t*)a, *(uint32_t*)b );
        }

        //! special case: 8-length
        template <> inline void bswap<8>(void *a,void *b) throw()
        {
            cswap( *(uint64_t*)a, *(uint64_t*)b );
        }
#endif

    }

    //! binary swap of types
    template <typename T>
    inline void bswap( T &a, T &b ) throw()
    {
        core::bswap<sizeof(T)>( (void *)&a, (void *)&b );
    }
    
    //! memory swap
    inline void mswap(void *a,void *b, const size_t n) throw()
    {
        uint8_t *A = (uint8_t *)a;
        uint8_t *B = (uint8_t *)b;
        for(size_t i=0;i<n;++i)
        {
            cswap(A[i],B[i]);
        }

    }


    //! memory reversal
    inline void mreverse( void *addr, const size_t size ) throw()
    {
        assert(!(addr==0&&size>0));
        uint8_t *p = static_cast<uint8_t*>(addr);
        uint8_t *q = static_cast<uint8_t*>(addr)+size;
        for(size_t i=(size>>1);i>0;--i)
        {
            cswap(*(p++), *(--q));
        }
    }

}


#endif

