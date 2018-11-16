//! \file
#ifndef Y_TYPE_BSWAP_INCLUDED
#define Y_TYPE_BSWAP_INCLUDED 1

#include "y/type/cswap.hpp"
#include "y/core/loop.hpp"

namespace upsylon
{

    
    namespace core
    {
        //! local byte swap
#define Y_BSWAP(I) const uint8_t tmp = p[I]; p[I] = q[I]; q[I] = tmp
        
        //! byte swapping
        template <const size_t N>
        inline void bswap( void *a, void *b ) throw()
        {
            uint8_t *p = static_cast<uint8_t *>(a);
            uint8_t *q = static_cast<uint8_t *>(b);
            Y_LOOP_FUNC_(N,Y_BSWAP,0);
        }
#undef  Y_BSWAP

        //! do nothing for no bytes
        template <>
        inline void bswap<0>(void*,void*) throw() {}
        
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

