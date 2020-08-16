
//! \file

#ifndef Y_SIBYL_TYPES_INCLUDED
#define Y_SIBYL_TYPE_INCLUDED 1

#include "y/type/ints.hpp"

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
            
            template <> inline
            void add_with_carry(uint8_t &carry, uint8_t &sum, const uint8_t lhs, const uint8_t rhs) throw()
            {
            }
        }
        
    }
    
}

#endif

