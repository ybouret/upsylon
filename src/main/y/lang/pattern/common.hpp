//! \file

#ifndef Y_LANG_PATTERN_COMMON_INCLUDED
#define Y_LANG_PATTERN_COMMON_INCLUDED 1

#include "y/lang/pattern/compiler.hpp"

namespace upsylon
{
    namespace Lang
    {
        //! common regular expressions
        struct Common
        {
            static const char INTEGER[];         //!< integer
            static const char POSITIVE_DOUBLE[]; //!< double without leading minus/plus sign
            static const char DOUBLE[];          //!< double without leading minus/plus sig
            static const char C_IDENTIFIER[];    //!< C/C++ ID
        };

    }
}

#endif

