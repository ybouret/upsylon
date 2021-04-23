
//! \file

#ifndef Y_IOS_UPACKER_INCLUDED
#define Y_IOS_UPACKER_INCLUDED 1

#include "y/code/ilog2.hpp"
#include "y/code/round.hpp"
#include "y/type/ints.hpp"

namespace upsylon {
    
    namespace ios {
        
        //! function to encode/decode an unsigned integral
        struct upacker
        {
            typedef uint64_t     word_type;                                                      //!< maximum word type to encode
            static const size_t  word_size   = sizeof(word_type);                                //!< maximum extra bytes
            static const size_t  word_bits   = word_size << 3;                                   //!< maximum word bits
            static const size_t  word_iln2   = ilog2<word_size>::value;                          //!< log2(word_size)
            static const size_t  prolog_bits = word_iln2+1;                                      //!< bits to encode word_size
            static const size_t  direct_bits = 8-prolog_bits;                                    //!< bits that don't need extra byte
            static const uint8_t prolog_mask = uint8_t(( (1<<prolog_bits) - 1) << direct_bits ); //!< extract prolog
            static const uint8_t direct_mask = uint8_t(( (1<<direct_bits) - 1) );                //!< extra direct bits, and maximum direct value
            
            //! functions to encode
            struct encode {

                //! shift 8 bits right for more than 2 bytes unsigned
                template <typename T> static inline void shr8(T &x, const int2type<true> &) throw()
                {
                    assert(sizeof(T)>1);
                    x >>= 8;
                }

                //! shift 8 bits right for one byte => 0
                template <typename T> static inline void shr8(T &x, const int2type<false> &) throw()
                {
                    assert(sizeof(T)==1);
                    x=0;
                }

                //! shit 8 bits right selector
                template <typename T> static inline void shr8(T &x) throw()
                {
                    static const int2type< (sizeof(T)>1) > choice = {};
                    shr8(x,choice);
                }


                //! compute the prolog and the requested extra bytes
                template <typename T> static inline
                uint8_t init( T &value, size_t &extra_bytes) throw()
                {
                    assert(0==extra_bytes);
                    size_t bits = bits_for(value); assert(bits<=word_bits);
                    if(bits<=direct_bits)
                    {
                        // nothing else to do
                        const uint8_t ans = uint8_t(value);
                        value       = 0;
                        return ans;
                    }
                    else
                    {
                        // compose prolog and compute remaining value
                        static const T direct_value_mask = direct_mask;
                        bits -= direct_bits; assert(bits>0);
                        extra_bytes = T(Y_ROUND8(bits) >> 3); assert(extra_bytes<=word_size);
                        const uint8_t ans = uint8_t( (extra_bytes<<direct_bits) | (value&direct_value_mask) );
                        value >>= direct_bits;
                        return ans;
                    }
                }
                
                //! get next byte from valid value
                template <typename T> static inline
                uint8_t next( T &value ) throw()
                {
                    static const T value_mask = 0xff;
                    assert(value>0);
                    const uint8_t ans = uint8_t(value&value_mask);
                    shr8(value);
                    return ans;
                }
                
            };
            
            //! functions to decode successive byte into an unsigned type
            struct decode {
             
                //! extract first bits and number of extra_bytes to read
                template <typename T> static inline
                size_t init(T             &value,
                            const uint8_t  first,
                            size_t        &shl) throw()
                {
                    value = T(first&direct_mask);
                    shl   = direct_bits;
                    return (first&prolog_mask) >> direct_bits; //! may be wrong!!
                }
                
                //! update value and shift
                template <typename T> static inline
                void next(T             &value,
                          const uint8_t  byte,
                          size_t         &shl) throw()
                {
                    T temp(byte);
                    temp <<= shl;
                    shl   += 8;
                    value |= temp;
                }
            };
            
            
            
        };
        
    }
}

#endif

