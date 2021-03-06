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
        typedef uint8_t      type;            //!< mapping
        static const uint8_t minimum = 0;     //!< min
        static const uint8_t maximum = 0xff;  //!< max
        //! check
        static inline type   positive(const type x) throw() { return x; }
    };
    
    //! will be uint16_t
    template <>
    struct unsigned_int<2> {
        typedef uint16_t      type;              //!< mapping
        static const uint16_t minimum = 0;       //!< min
        static const uint16_t maximum = 0xffff;  //!< max
        //! check
        static inline type    positive(const type x) throw() { return x; }
    };
    
    //! will be uint32_t
    template <>
    struct unsigned_int<4> {
        typedef uint32_t      type;                 //!< mapping
        static const uint32_t minimum = 0;          //!< min
        static const uint32_t maximum = 0xffffffff; //!< max
        //! check
        static inline type    positive(const type x) throw() { return x; }
    };
    
    //! will be uint64_t
    template <>
    struct unsigned_int<8> {
        typedef uint64_t type;                                     //!< mapping
        static const uint64_t minimum = 0;                         //!< min
        static const uint64_t maximum = Y_U64(0xffffffffffffffff); //!< max
        //! check
        static inline type    positive(const type x) throw() { return x; }
    };
    
    //! compute signed int of a given size
    template <const size_t N>
    struct signed_int;
    
    //! will be int8_t
    template <>
    struct signed_int<1> {
        typedef int8_t      type;                    //!< mapping
        static const int8_t maximum =  127;          //!< min
        static const int8_t minimum = (-maximum-1);  //!< max
        //! check
        static inline type  positive(const type x) throw() { return (x<0)?0:x; }
    };
    
    //! will be int16_t
    template <>
    struct signed_int<2> {
        typedef int16_t      type;                     //!< mapping
        static const int16_t maximum =  32767;         //!< max
        static const int16_t minimum = (-maximum-1);   //!< min
        //! check
        static inline type   positive(const type x) throw() { return (x<0)?0:x; }
    };
    
    //! will be int32_t
    template <>
    struct signed_int<4> {
        typedef int32_t      type;                   //!< mapping
        static const int32_t maximum =  2147483647;  //!< max
        static const int32_t minimum = (-maximum-1); //!< min
        //! check
        static inline type   positive(const type x) throw() { return (x<0)?0:x; }
    };
    
    //! will be int64_t
    template <>
    struct signed_int<8> {
        typedef int64_t      type;                                 //!< mapping
        static const int64_t maximum =  Y_U64(0x7fffffffffffffff); //!< max
        static const int64_t minimum =  (-maximum-1);              //!< min
        //! check
        static inline type   positive(const type x) throw() { return (x<0)?0:x; }
    };
    
    //! compute if an integral type is signed
    template <typename T>
    struct is_signed {
        static const bool value = (T(-1) < T(0)); //!< compile time detection
    };

    //! finding matching integral type
    template <typename T>
    struct integral_of
    {
        enum            { sizeof_T = sizeof(T) };         //!< alias
        static const bool signed_T = is_signed<T>::value; //!< alias

        typedef unsigned_int<sizeof_T>                                 u_int_T;       //!< the matching unsigned struct, in any case
        typedef signed_int<sizeof_T>                                   s_int_T;       //!< the matching signed struct
        typedef typename select_type<signed_T,s_int_T,u_int_T>::result __integral;    //!< the matching struct
        typedef typename __integral::type                              type;          //!< the matching type
    };

    //! compute the limits on an integral type
    template <typename T>
    struct limit_of
    {
        enum            { sizeof_T = sizeof(T) };         //!< alias
        static const bool signed_T = is_signed<T>::value; //!< alias

        typedef unsigned_int<sizeof_T>                                 u_int_T;       //!< the matching unsigned struct, in any case
        typedef signed_int<sizeof_T>                                   s_int_T;       //!< the matching signed struct
        typedef typename select_type<signed_T,s_int_T,u_int_T>::result __integral;    //!< the matching integral struct


        typedef typename __integral::type  integral_type;  //!< the matching integer
        typedef typename u_int_T::type     unsigned_type;  //!< the matching unsigned

        static const integral_type minimum = __integral::minimum; //!< associated min
        static const integral_type maximum = __integral::maximum; //!< associated max

        //! check
        static inline unsigned_type positive( const integral_type x ) throw()
        {
            return static_cast<unsigned_type>( __integral::positive(x));
        }

    };

    //! bytes for an unsigned type
    template <typename U>
    inline uint8_t bytes_for(const U n) throw()
    {
        static const uint8_t max_bytes = sizeof(U);
        U mask = static_cast<U>(0xff) << ( (max_bytes-1) * 8 );
        for( uint8_t i=max_bytes;i>0;--i)
        {
            if( (n&mask) != 0 )
            {
                return i;
            }
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
            if( (n & mask) )
                return i;
            mask >>= 1;
        }
        return 0;
    }

    //! bits table
    struct bits_table
    {
        static const uint8_t  count_for_byte[256]; //!< count of bits for bytes[0..255]
        static const uint8_t  value[8];            //!< [2^0...2^7]
        static const uint8_t  vmask[8];            //!< ~value=255-value
        static const uint8_t  _true[256];          //!< [0,1,1,1,1,1,1...1,1,1]
    };

  

}

#endif

