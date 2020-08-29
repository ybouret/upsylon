
//! \file

#ifndef Y_PATTERN_BASIC_REWORK_INCLUDED
#define Y_PATTERN_BASIC_REWORK_INCLUDED 1

#include "y/jive/pattern/basic/all.hpp"

namespace upsylon {

    namespace Jive
    {
        //______________________________________________________________________
        //
        //
        //! reworking out of order logical
        //
        //______________________________________________________________________
        class Rework
        {
        public:
            //! compact basic operands of rewritable
            static void Compact( Pattern::List &ops ) throw();

            //! in-place transformation of a Single into a Range
            static Pattern *Single2Range(Single *p, const uint8_t lower, const uint8_t upper) throw();


        };

    }

}

#endif
