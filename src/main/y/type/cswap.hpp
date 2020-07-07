//! \file
#ifndef Y_TYPE_CSWAP_INCLUDED
#define Y_TYPE_CSWAP_INCLUDED 1

#include "y/os/platform.hpp"
namespace upsylon
{
    //! C-style swap
    template <typename T>
    inline void cswap(T &a, T &b)
    {
        const T tmp(a);
        a=b;
        b=tmp;
    }

    //! C-style forced swap, DANGEROUS
    template <typename T>
    inline void _cswap(const T &a, const T &b)
    {
        const T tmp(a);
        (T&)a=b;
        (T&)b=tmp;
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

    //! assert increasing
    template <typename T>
    void cswap_increasing(T &a, T &b)
    {
        if(b<a) cswap(a,b); assert(a<=b);
    }

    //! assert decreasing
    template <typename T>
    void cwap_decreasing(T &a, T &b)
    {
        if(a<b) cswap(a,b); assert(b<=a);
    }

}

#endif

