
//! \file

#ifndef Y_SIBYL_TYPES_INCLUDED
#define Y_SIBYL_TYPE_INCLUDED 1

#include "y/type/ints.hpp"
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
                    } r;
                };
                half L = { lhs }; assert(L.w==lhs);
                half R = { rhs }; assert(R.w==rhs);
                uint8_t  slo = 0;
                uint8_t  shi = 0;
                add4(carry,slo,L.r.lo,R.r.lo);
                add4(carry,shi,L.r.hi,R.r.hi);
                sum = slo | (shi<<4);
            }
            
            template <> inline
            void add_with_carry(uint16_t &carry, uint16_t &sum, const uint16_t lhs, const uint16_t rhs) throw()
            {
                std::cerr << "not implemented" << std::endl;
                exit(-1);
            }
            
        }
        
    }
    
}

#endif

