//! \file
#ifndef Y_LANG_PATTERN_COMPILER_INCLUDED
#define Y_LANG_PATTERN_COMPILER_INCLUDED 1

#include "y/lang/pattern/dictionary.hpp"

namespace upsylon
{
    namespace Lang
    {
        struct Compile
        {
            static Pattern *RegEx( const string &rx, const Dictionary *dict);
            
        };
    }
}

#endif

