
//! \file

/**
 Simple Inline Bignum Uspylon Library
 */
#ifndef Y_SIBYL_NATURAL_INCLUDED
#define Y_SIBYL_NATURAL_INCLUDED 1

#include "y/sibyl/number.hpp"
#include "y/code/round.hpp"

namespace upsylon {


    namespace sibyl {

        template <size_t WORD_BITS>
        class natural : public number
        {
        public:
            static const size_t                            word_bits = Y_ROUND8(WORD_BITS);
            static const size_t                            word_size = word_bits >> 3;
            typedef typename unsigned_int<word_size>::type word_type;
            static  const size_t                           min_core_size = word_size << 1;
            static  const size_t                           core_size     = (min_core_size>sys_core_size) ? max_core_size : sys_core_size;
            static  const bool                             possible      = core_size>=min_core_size;
            

        private:

        };

    }

}


#endif

