//! \file
#ifndef Y_OS_ENDIAN_INCLUDED
#define Y_OS_ENDIAN_INCLUDED 1

#include "y/type/ints.hpp"
#include <cstring>

#if defined(Y_BYTE_ORDER)
#    error "Y_BYTE_ORDER should NOT be defined"
#endif

//! big endian style
#define Y_BIG_ENDIAN 0x1234
//! little endian style
#define Y_LIT_ENDIAN 0x3412

#if defined(__GNUC__)
#    if defined(__i386__) || defined(__x86_64__) || defined(__ia64__)
#        define Y_BYTE_ORDER Y_LIT_ENDIAN
#    endif
#    if defined(__ppc__)
#        define Y_BYTE_ORDER Y_BIG_ENDIAN
#    endif
#endif

#if defined(_MSC_VER)
#        define Y_BYTE_ORDER Y_LIT_ENDIAN
#endif

#if defined(__DMC__)
#        define Y_BYTE_ORDER Y_LIT_ENDIAN
#endif

#if !defined(Y_BYTE_ORDER)
#    error "Y_BYTE_ORDER could not be guessed!"
#endif

//! no swap for 16 bits
#define Y_NOPE16(X) ((uint16_t)(X))

//! in place swap for 16 bits
#define Y_SWAP16(X)                \
(                                      \
( (((uint16_t)(X)) & 0xff00 ) >> 8 ) | \
( (((uint16_t)(X)) & 0x00ff ) << 8 )   \
)

//! no swap for 32 bits
#define Y_NOPE32(X) ((uint32_t)(X))

//! in place swap for 16 bits
#define Y_SWAP32(X)                \
(                                      \
(((uint32_t)(X) & 0xff000000) >> 24) | \
(((uint32_t)(X) & 0x00ff0000) >> 8)  | \
(((uint32_t)(X) & 0x0000ff00) << 8)  | \
(((uint32_t)(X) & 0x000000ff) << 24)   \
)

//! no swap for 64 bits
#define Y_NOPE64(X) ((uint64_t)(X))

//! helper for 64 bits
#define Y_MASK64(N) ( ( (uint64_t)(0xff) ) << (N) )

//! in place swap for 64 bits
#define Y_SWAP64(X)                       \
(                                             \
(((uint64_t)(X) & Y_MASK64(56) ) >> 56) | \
(((uint64_t)(X) & Y_MASK64(48) ) >> 40) | \
(((uint64_t)(X) & Y_MASK64(40) ) >> 24) | \
(((uint64_t)(X) & Y_MASK64(32) ) >>  8) | \
(((uint64_t)(X) & Y_MASK64(24) ) <<  8) | \
(((uint64_t)(X) & Y_MASK64(16) ) << 24) | \
(((uint64_t)(X) & Y_MASK64( 8) ) << 40) | \
(((uint64_t)(X) & Y_MASK64( 0) ) << 56)   \
)


#if Y_BYTE_ORDER == Y_BIG_ENDIAN

//! big endian swap 16 bits/big endian arch
#define Y_SWAP_BE16(X) Y_NOPE16(X)

//! little endian swap 16 bits/big endian arch
#define Y_SWAP_LE16(X) Y_SWAP16(X)

//! big endian swap 32 bits/big endian arch
#define Y_SWAP_BE32(X) Y_NOPE32(X)

//! little endian swap 32 bits/big endian arch
#define Y_SWAP_LE32(X) Y_SWAP32(X)

//! big endian swap 64 bits/big endian arch
#define Y_SWAP_BE64(X) Y_NOPE64(X)

//! little endian swap 64 bits/big endian arch
#define Y_SWAP_LE64(X) Y_SWAP64(X)

#else

//! big endian swap 16 bits/little endian arch
#define Y_SWAP_BE16(X) Y_SWAP16(X)

//! little endian swap 16 bits/little endian arch
#define Y_SWAP_LE16(X) Y_NOPE16(X)

//! big endian swap 32 bits/little endian arch
#define Y_SWAP_BE32(X) Y_SWAP32(X)

//! little endian swap 32 bits/little endian arch
#define Y_SWAP_LE32(X) Y_NOPE32(X)

//! big endian swap 64 bits/little endian arch
#define Y_SWAP_BE64(X) Y_SWAP64(X)

//! little endian swap 64 bits/little endian arch
#define Y_SWAP_LE64(X) Y_NOPE64(X)

#endif

namespace upsylon {

    //! swap big endian 16 bits
    inline uint16_t swap_be16( uint16_t x ) throw() {
        return uint16_t(Y_SWAP_BE16(x));
    }

    //! swap little endian 16 bits
    inline uint16_t swap_le16( uint16_t x ) throw() {
        return Y_SWAP_LE16(x);
    }

    //! swap big endian 32 bits
    inline uint32_t swap_be32( uint32_t x ) throw() {
        return Y_SWAP_BE32(x);
    }

    //! swap little endian 32 bits
    inline uint32_t swap_le32( uint32_t x ) throw() {
        return Y_SWAP_LE32(x);
    }

    //! swap big endian 64 bits
    inline uint64_t swap_be64( uint64_t x ) throw() {
        return Y_SWAP_BE64(x);
    }

    //! swap little endian 64 bits
    inline uint64_t swap_le64( uint64_t x ) throw() {
        return Y_SWAP_LE64(x);
    }

    //! type agnostic swap big endian
    template <class T> T swap_be( const T &x ) throw();

    //! 8 bits swap big endian
    template <>
    inline uint8_t swap_be( const uint8_t &x ) throw() {
        return x;
    }


    //! 16 bits swap big endian
    template <>
    inline uint16_t swap_be( const uint16_t &x ) throw() {
        return swap_be16(x);
    }

    //! 32 bits swap big endian
    template <>
    inline uint32_t swap_be( const uint32_t &x ) throw() {
        return swap_be32(x);
    }

    //! 64 bits swap big endian
    template <>
    inline uint64_t swap_be( const uint64_t &x ) throw() {
        return swap_be64(x);
    }

    //! big endian swapping of integral type
    template <typename T>
    inline T swap_be_as( T x ) throw()
    {
        typedef typename unsigned_int<sizeof(T)>::type uint_T;
        union {
            T      item;
            uint_T bits;
        } alias = { x };
        alias.bits = swap_be<uint_T>(alias.bits);
        return alias.item;
    }

    //! store an integral type in big endian form at address
    template <typename T>
    inline void store_be( T x, void *addr ) throw()
    {
        assert(addr!=0);
        typedef typename unsigned_int<sizeof(T)>::type uint_T;
        union {
            T      item;
            uint_T bits;
        } alias = { x };
        const uint_T y = swap_be<uint_T>(alias.bits);
        const char  *q = (const char *)&y;
        char        *p = (char       *)addr;
        memcpy(p,q,sizeof(T));
    }

    //! query a big endian stored data at address
    template <typename T>
    inline T query_be(const void *addr) throw()
    {
        assert(addr!=0);
        typedef typename unsigned_int<sizeof(T)>::type uint_T;
        union {
            uint_T bits;
            T      item;
        } alias = { 0 };
        const char *q = (const char*)addr;
        char       *p = (char *) &alias.bits;
        memcpy(p,q,sizeof(T));
        alias.bits = swap_be<uint_T>(alias.bits);
        return alias.item;
    }

    //__________________________________________________________________________
    //
    // little endian swapping
    //__________________________________________________________________________

    //! type agnostic swap little endian
    template <class T> T swap_le( const T &x ) throw();

    //! 8 bits swap little endian
    template <>
    inline uint8_t swap_le( const uint8_t &x ) throw() {
        return x;
    }

    //! 16 bits swap little endian
    template <>
    inline uint16_t swap_le( const uint16_t &x ) throw() {
        return swap_le16(x);
    }

    //! 32 bits swap little endian
    template <>
    inline uint32_t swap_le( const uint32_t &x ) throw() {
        return swap_le32(x);
    }

    //! 64 bits swap little endian
    template <>
    inline uint64_t swap_le( const uint64_t &x ) throw() {
        return swap_le64(x);
    }

    //! little endian swapping of integral type
    template <typename T>
    inline T swap_le_as( T x ) throw()
    {
        typedef typename unsigned_int<sizeof(T)>::type uint_T;
        union {
            T      item;
            uint_T bits;
        } alias = { x };
        alias.bits = swap_le<uint_T>(alias.bits);
        return alias.item;
    }


}


#endif
