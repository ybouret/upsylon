//! \file
#ifndef Y_TYPE_BSWAP_INCLUDED
#define Y_TYPE_BSWAP_INCLUDED 1

#include "y/type/cswap.hpp"
#include "y/type/ints.hpp"
#include "y/core/loop.hpp"

namespace upsylon
{

    namespace core
    {
        typedef unsigned_int<sizeof(ptrdiff_t)>::type bswap_t; //!< word type for bswap

        //! inline type exchange
#define Y_BSWAP_TYPE(T,I)  const T tmp = A[I]; A[I] = B[I]; B[I] = tmp
        //! inline block exchange
#define Y_BSWAP_BLOCK(I)  Y_BSWAP_TYPE(bswap_t,I)
        //! inline byte exchange
#define Y_BSWAP_BYTE(I)   Y_BSWAP_TYPE(uint8_t,I)

        //! swap BLOCKS blocks
        template <const size_t BLOCKS>
        inline void bswap_blocks(bswap_t *A, bswap_t *B) throw()
        {
            Y_LOOP_FUNC_STATIC_(BLOCKS,Y_BSWAP_BLOCK,0);
        }

        //! swap no blocks
        template < > inline void bswap_blocks<0>(bswap_t *, bswap_t *) throw() {}

        //! swap BYTES bytes
        template <const size_t BYTES>
        inline void bswap_bytes(void *a, void *b) throw()
        {
            uint8_t *A = (uint8_t *)a;
            uint8_t *B = (uint8_t *)b;
            Y_LOOP_FUNC_STATIC_(BYTES,Y_BSWAP_BYTE,0);
        }

        //! swap no BYTES
        template <> inline void bswap_bytes<0>(void *, void *) throw() {}

        //! swap N= Q blocks+R bytes
        template <const size_t N>
        void bswap(void *a,void *b) throw()
        {

            static const size_t Q = N/sizeof(bswap_t);
            static const size_t R = N&(sizeof(bswap_t)-1);

            bswap_t *A = static_cast<bswap_t *>(a);
            bswap_t *B = static_cast<bswap_t *>(b);
            bswap_blocks<Q>(A,B);
            bswap_bytes<R>(A+Q,B+Q);

        }

    }

    //! binary swap of tyoes
    template <typename T>
    void bswap( T &a, T &b ) throw()
    {
        core::bswap<sizeof(T)>( &a, &b );
    }
    
    //! memory swap
    inline void mswap(void *a,void *b, const size_t n) throw()
    {
        uint8_t *A = (uint8_t *)a;
        uint8_t *B = (uint8_t *)b;
        Y_LOOP_FUNC_(n,Y_BSWAP_BYTE,0);
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

