
//! \file

#ifndef Y_PATTERN_BASIC_REWORK_INCLUDED
#define Y_PATTERN_BASIC_REWORK_INCLUDED 1

#include "y/jive/pattern/basic/all.hpp"

namespace upsylon {

    namespace Jive
    {

        class Rework
        {
        public:
            //! compact out of order operands
            static void Compact( Pattern::List &ops ) throw();

            static Pattern *Single2Range(Single *p, const uint8_t lower, const uint8_t upper) throw();


        };

    }

}

#endif
