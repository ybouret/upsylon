//! \file
#ifndef Y_TYPE_BSWAP_INCLUDED
#define Y_TYPE_BSWAP_INCLUDED 1

#include "y/type/cswap.hpp"
#include "y/core/loop.hpp"

namespace upsylon
{

    
    namespace core
    {
        //______________________________________________________________________
        //
        // Generic case
        //______________________________________________________________________

        //! called in loop
#define Y_BSWAP64(I) const uint64_t tmp = p[I]; p[I] = q[I]; q[I] = tmp

        template <const size_t Q>
        inline void bswap64(uint64_t *p, uint64_t *q) throw()
        {
            Y_LOOP_FUNC_(Q,Y_BSWAP64,0);
        }

        template <>
        inline void bswap64<0>(uint64_t *, uint64_t *) throw()
        {
        }

        //! byte swapping
        template <const size_t N>
        void bswap( void *a, void *b ) throw()
        {
            static const size_t Q = (N>>3);
            static const size_t R = (N&7);
            uint64_t *p = (uint64_t *)a;
            uint64_t *q = (uint64_t *)b;
            bswap64<Q>(p,q);
            bswap<R>(p+Q,q+Q);
        }




        //______________________________________________________________________
        //
        // full word
        //______________________________________________________________________

        template <typename T>
        inline void cast_swap(void *a,void *b) throw()
        {
            const T tmp = *static_cast<T*>(a);
            *static_cast<T*>(a)= *static_cast<T*>(b);
            *static_cast<T*>(b)=tmp;;
        }

        //! do nothing for no bytes
        template <> inline void bswap<0>(void*,void*)   throw() {}
        template <> inline void bswap<1>(void*a,void*b) throw() { cast_swap<uint8_t>(a,b);  }
        template <> inline void bswap<2>(void*a,void*b) throw() { cast_swap<uint16_t>(a,b); }
        template <> inline void bswap<4>(void*a,void*b) throw() { cast_swap<uint32_t>(a,b); }

        template <> inline void bswap<3>(void*a,void*b) throw() {}
        template <> inline void bswap<5>(void*a,void*b) throw() {}
        template <> inline void bswap<6>(void*a,void*b) throw() {}
        template <> inline void bswap<7>(void*a,void*b) throw() {}



        
    }
    
    //! swapping memory contents
    template <typename T>
    inline void bswap( T &a, T &b ) throw()
    {
        core::bswap<sizeof(T)>(&a,&b);
    }

    //! local byte swap
#define Y_MSWAP(I) const uint8_t tmp = P[I]; P[I] = Q[I]; Q[I] = tmp

    //! any memory swap
    inline void mswap( void *p, void *q, const size_t size ) throw()
    {
        assert(size>0);
        uint8_t *P = static_cast<uint8_t *>(p);
        uint8_t *Q = static_cast<uint8_t *>(q);
        Y_LOOP_FUNC_(size,Y_MSWAP,0);
    }
    
#undef Y_MSWAP

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

