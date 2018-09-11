//! \file
#ifndef Y_LANG_PATTERN_COMPILER_INCLUDED
#define Y_LANG_PATTERN_COMPILER_INCLUDED 1

#include "y/lang/pattern/dictionary.hpp"

namespace upsylon
{
    namespace Lang
    {
        //! Compiling Regular Expressions
        struct Compile
        {
            //! compile a regular expression
            static Pattern *RegEx( const string &rx, const Dictionary *dict=NULL);
            //! compile a regular expression
            inline
            static Pattern *RegEx(const char *rx, const Dictionary *dict=NULL)
            {
                const string _ = rx; return RegEx(_,dict);
            }
        };
    }
}

#endif

