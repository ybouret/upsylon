
//! \file

#ifndef Y_AP_PRIME_FACTORS_INCLUDED
#define Y_AP_PRIME_FACTORS_INCLUDED 1

#include "y/yap/prime/factor.hpp"
#include "y/core/roll.hpp"

namespace upsylon {

    namespace yap {

        class prime_factors : public number
        {
        public:
            typedef core::roll_of<prime_factor> roll_type;

            explicit prime_factors() throw();
            virtual ~prime_factors() throw();


            const roll_type factors;
        };

    }

}

#endif
