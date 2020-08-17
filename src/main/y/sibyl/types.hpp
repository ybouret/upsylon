
//! \file

#ifndef Y_SIBYL_TYPES_INCLUDED
#define Y_SIBYL_TYPES_INCLUDED 1

#include "y/type/ints.hpp"
#include "y/os/endian.hpp"
#include <iostream>
namespace upsylon
{
    
    namespace sibyl
    {
        
        namespace slick
        {
            
            template <typename CORE,typename WORD> inline
            void add_with_carry(CORE &carry, CORE &sum, const WORD lhs, const WORD rhs) throw()
            {
                static const unsigned word_bits = sizeof(WORD)<<3;
                carry += CORE(lhs) + CORE(rhs);
                sum    = WORD(carry);
                carry >>= word_bits;
            }
            
            inline void add4(uint8_t &carry, uint8_t &sum, const uint8_t lhs, const uint8_t rhs) throw()
            {
                carry += lhs + rhs;
                sum    = carry & 0x0f;
                carry >>= 4;
            }
            
            template <> inline
            void add_with_carry(uint8_t &carry, uint8_t &sum, const uint8_t lhs, const uint8_t rhs) throw()
            {
                union half
                {
                    uint8_t w;
                    struct {
                        uint8_t lo : 4;
                        uint8_t hi : 4;
                    } h;
                };
                half L = { lhs }; assert(L.w==lhs);
                half R = { rhs }; assert(R.w==rhs);
                uint8_t  slo = 0;
                uint8_t  shi = 0;
                add4(carry,slo,L.h.lo,R.h.lo);
                add4(carry,shi,L.h.hi,R.h.hi);
                sum = slo | (shi<<4);
            }
            
            template <> inline
            void add_with_carry(uint16_t &carry, uint16_t &sum, const uint16_t lhs, const uint16_t rhs) throw()
            {
                const uint8_t L_lo = uint8_t(lhs);
                const uint8_t L_hi = uint8_t(lhs>>8);
                const uint8_t R_lo = uint8_t(rhs);
                const uint8_t R_hi = uint8_t(rhs>>8);
                
                uint16_t  slo = 0;
                uint16_t  shi = 0;
                add_with_carry(carry,slo,L_lo,R_lo);
                add_with_carry(carry,shi,L_hi,R_hi);
                sum = slo | (shi<<8);
            }
            
            template <> inline
            void add_with_carry(uint32_t &carry, uint32_t &sum, const uint32_t lhs, const uint32_t rhs) throw()
            {
                const uint16_t L_lo = uint16_t(lhs);
                const uint16_t L_hi = uint16_t(lhs>>16);
                const uint16_t R_lo = uint16_t(rhs);
                const uint16_t R_hi = uint16_t(rhs>>16);
                
                uint32_t  slo = 0;
                uint32_t  shi = 0;
                add_with_carry(carry,slo,L_lo,R_lo);
                add_with_carry(carry,shi,L_hi,R_hi);
                sum = slo | (shi<<16);
            }
            
            template <> inline
            void add_with_carry(uint64_t &carry, uint64_t &sum, const uint64_t lhs, const uint64_t rhs) throw()
            {
                const uint32_t L_lo = uint16_t(lhs);
                const uint32_t L_hi = uint16_t(lhs>>32);
                const uint32_t R_lo = uint16_t(rhs);
                const uint32_t R_hi = uint16_t(rhs>>32);
                
                uint64_t  slo = 0;
                uint64_t  shi = 0;
                add_with_carry(carry,slo,L_lo,R_lo);
                add_with_carry(carry,shi,L_hi,R_hi);
                sum = slo | (shi<<32);
            }
            
            
            
            
        }
        
    }
    
}

#endif

