//! \file
#ifndef Y_ROUND_INCLUDED
#define Y_ROUND_INCLUDED 1

#include "y/code/ilog2.hpp"

//! get rounding byte=2^LN2
#define Y_ROUND_BYTE( LN2 ) ( static_cast<size_t>( 1 << (LN2) ) )

//! get rounding mask=(2^LN2-1)
#define Y_ROUND_MASK( LN2 ) ( static_cast<size_t>(Y_ROUND_BYTE(LN2)-0x1) )

//! get rounding mask 2-complement
#define Y_ROUND_NOT2(LN2) (~Y_ROUND_MASK(LN2) )

//! get the rounded value
#define Y_ROUND_LN2(LN2,N) ( (static_cast<size_t>(N) + Y_ROUND_MASK(LN2)) & Y_ROUND_NOT2(LN2) )

//! 1 byte  | 8 bits
#define Y_ROUND1(N)    Y_ROUND_LN2(0,N)

//! 2 bytes  | 16 bits
#define Y_ROUND2(N)    Y_ROUND_LN2(1,N)

//! 4 bytes  | 32 bits
#define Y_ROUND4(N)    Y_ROUND_LN2(2,N)

//! 8 bytes  | 64 bits
#define Y_ROUND8(N)    Y_ROUND_LN2(3,N)

//! 16 bytes | 128 bits
#define Y_ROUND16(N)   Y_ROUND_LN2(4,N)

//! 32 bytes | 256 bits
#define Y_ROUND32(N)   Y_ROUND_LN2(5,N)

//! 64 bytes | 512 bits
#define Y_ROUND64(N)   Y_ROUND_LN2(6,N)

//! 128 bytes | 1024 bits
#define Y_ROUND128(N)  Y_ROUND_LN2(7,N)

//! 256 bytes | 2048 bits
#define Y_ROUND256(N)  Y_ROUND_LN2(8,N)

//! 512 bytes | 4096 bits
#define Y_ROUND512(N)  Y_ROUND_LN2(9,N)

//! internal memory alignment
#define Y_MEMORY_ALIGN(N)  Y_ROUND16(N)

//! number of uint64_t to hold N
#define Y_U64_FOR_SIZE(N) ( Y_ROUND8(N) >> 3 )

//! number of uint64_t to hold T
#define Y_U64_FOR_ITEM(T) Y_U64_FOR_SIZE(sizeof(T))

//! number of bytes to describe some bits
#define Y_BYTES_FOR(BITS) ( (Y_ROUND_LN2(3,BITS)) >> 3 )

//! align on a power of two BYTES
#define Y_ALIGN_FOR_SIZE(BYTES,N) Y_ROUND_LN2(ilog2<BYTES>::value,N)

//! align on a primitive type, which has a power of two size
#define Y_ALIGN_FOR_ITEM(T,N)     Y_ROUND_LN2(ilog2_of<T>::value,N)

namespace upsylon
{
    namespace memory
    {
        //! function for memory alignment
        inline size_t align(const size_t n) throw() { return Y_MEMORY_ALIGN(n); }
    }
}

#endif
