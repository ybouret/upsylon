//! \file
#ifndef Y_ROUND_INCLUDED
#define Y_ROUND_INCLUDED 1

#include "y/code/ilog2.hpp"

//! get rounding byte
#define Y_ROUND_BYTE( ROUND_LN2 ) ( (size_t)( 1 << (ROUND_LN2) ) )
//! get rounding mask
#define Y_ROUND_MASK( ROUND_LN2 ) ( (size_t)(Y_ROUND_BYTE( ROUND_LN2 )-0x1) )
//! get rounding mask 2-complement
#define Y_ROUND_NOT2( ROUND_LN2 ) ( (size_t)(~Y_ROUND_MASK( ROUND_LN2 ) )   )
//! get the rounded value
#define Y_ROUND(ROUND_LN2,N)      ( ( (size_t)(N) + Y_ROUND_MASK(ROUND_LN2) ) & Y_ROUND_NOT2( ROUND_LN2 ) )

//! 1 byte  | 8 bits
#define Y_ROUND1(N)    Y_ROUND(0,N)

//! 2 bytes  | 16 bits
#define Y_ROUND2(N)    Y_ROUND(1,N)

//! 4 bytes  | 32 bits
#define Y_ROUND4(N)    Y_ROUND(2,N)

//! 8 bytes  | 64 bits
#define Y_ROUND8(N)    Y_ROUND(3,N)

//! 16 bytes | 128 bits
#define Y_ROUND16(N)   Y_ROUND(4,N)

//! 32 bytes | 256 bits
#define Y_ROUND32(N)   Y_ROUND(5,N)

//! 64 bytes | 512 bits
#define Y_ROUND64(N)   Y_ROUND(6,N)

//! 128 bytes | 1024 bits
#define Y_ROUND128(N)  Y_ROUND(7,N)

//! 256 bytes | 2048 bits
#define Y_ROUND256(N)  Y_ROUND(8,N)

//! internal memory alignment
#define Y_MEMALIGN(N)  Y_ROUND16(N)

//! number of uint64_t to hold N
#define Y_U64_FOR_SIZE(N) ( Y_ROUND8(N) >> 3 )

//! number of uint64_t to hold T
#define Y_U64_FOR_ITEM(T) Y_U64_FOR_SIZE(sizeof(T))

//! number of uin32_t to hold N
#define Y_U32_FOR_SIZE(N) ( Y_ROUND4(N) >> 2 )

//! number of uint32_t to hold T
#define Y_U32_FOR_ITEM(T) Y_U32_FOR_SIZE(sizeof(T))

//! number of bytes to describe some bits
#define Y_BYTES_FOR(BITS) ( (size_t)(Y_ROUND(3,BITS)) >>3 )

//! align on a power of two BYTES
#define Y_ALIGN_FOR_SIZE(BYTES,N) Y_ROUND(ilog2<BYTES>::value,N)

//! align on a primitive type, which has a power of two size
#define Y_ALIGN_FOR_ITEM(T,N)     Y_ROUND(ilog2_of<T>::value,N)

namespace upsylon
{
    namespace memory
    {
        //! function for memory alignment
        inline size_t align( size_t n ) throw() { return Y_MEMALIGN(n); }
    }
}

#endif
