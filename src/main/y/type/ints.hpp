//! \file
#ifndef Y_TYPE_LIMITS_INCLUDED
#define Y_TYPE_LIMITS_INCLUDED 1

#include "y/type/select.hpp"

namespace upsylon
{
    //! compute unsigned int of a given size
    template <const size_t N>
    struct unsigned_int ;
    
    //! will be uint8_t
    template <>
    struct unsigned_int<1> {
        typedef uint8_t      type; //!< mapping
        static const uint8_t minimun = 0;     //!< min
        static const uint8_t maximum = 0xff;  //!< max
    };
    
    //! will be uint16_t
    template <>
    struct unsigned_int<2> {
        typedef uint16_t      type; //!< mapping
        static const uint16_t minimun = 0;       //!< min
        static const uint16_t maximum = 0xffff;  //!< max
    };
    
    //! will be uint32_t
    template <>
    struct unsigned_int<4> {
        typedef uint32_t      type; //!< mapping
        static const uint32_t minimun = 0;          //!< min
        static const uint32_t maximum = 0xffffffff; //!< max
    };
    
    //! will be uint64_t
    template <>
    struct unsigned_int<8> {
        typedef uint64_t type; //!< mapping
        static const uint64_t minimun = 0;                         //!< min
        static const uint64_t maximum = Y_U64(0xffffffffffffffff); //!< max
    };
    
    //! compute signed int of a given size
    template <const size_t N>
    struct signed_int;
    
    //! will be int8_t
    template <>
    struct signed_int<1> {
        typedef int8_t type; //!< mapping
        static const int8_t maximum =  127;          //!< min
        static const int8_t minimun = (-maximum-1);  //!< max
    };
    
    //! will be int16_t
    template <>
    struct signed_int<2> {
        typedef int16_t type; //!< mapping
        static const int16_t maximum =  32767;         //!< min
        static const int16_t minimun = (-maximum-1);   //!< max
        
    };
    
    //! will be int32_t
    template <>
    struct signed_int<4> {
        typedef int32_t type; //!< mapping
        static const int32_t maximum =  2147483647;  //!< min
        static const int32_t minimun = (-maximum-1); //!< ma
        
    };
    
    //! will be int64_t
    template <>
    struct signed_int<8> {
        typedef int64_t type; //!< mapping
        static const int64_t maximum =  Y_U64(0x7fffffffffffffff); //!< min
        static const int64_t minimun =  (-maximum-1);              //!< max
        
    };
    
    //! compute if an integral type is signed
    template <typename T>
    struct is_signed {
        static const bool value = (T(-1) < T(0)); //!< compile time detection
    };
    
    //! compute the limits on an integral type
    template <typename T>
    struct limit_of
    {
        enum { size_of = sizeof(T) }; //!< alias
        typedef typename select_type< is_signed<T>::value,
        signed_int<size_of>,
        unsigned_int<size_of>
        >::result __integer; //!< the matching integral struct
        
        static const typename __integer::type minimun = __integer::minimun; //!< associated min
        static const typename __integer::type maximum = __integer::maximum; //!< associated max
    };

    //! bytes for an unsigned type
    template <typename U>
    inline size_t bytes_for(const U n) throw()
    {
        static const size_t max_bytes = sizeof(U);
        U mask = static_cast<U>(0xff) << ( (max_bytes-1) * 8 );
        for( size_t i=max_bytes;i>0;--i)
        {
            if( (n & mask) )
                return i;
            mask >>= 8;
        }
        return 0;
    }

    //! bits for an unsigned type
    template <typename U>
    inline size_t bits_for(const U n) throw()
    {
        const size_t max_bits = sizeof(U) * 8;
        U mask = static_cast<U>(0x01) << ( max_bits-1 );
        for( int i=max_bits;i>0;--i)
        {
            if( n & mask )
                return i;
            mask >>= 1;
        }
        return 0;
    }


}

#endif

