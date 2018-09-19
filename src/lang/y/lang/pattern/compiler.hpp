//! \file
#ifndef Y_LANG_PATTERN_COMPILER_INCLUDED
#define Y_LANG_PATTERN_COMPILER_INCLUDED 1

#include "y/lang/pattern/dictionary.hpp"

namespace upsylon
{
    namespace Lang
    {
        class RXCompiler; //! forward declaration

        //! Compiling Regular Expressions
        struct Compile
        {
            //! compile a regular expression
            static Pattern *RegExp( const string &rx, const Dictionary *dict=NULL);
            
            //! compile a regular expression
            inline
            static Pattern *RegExp(const char *rx, const Dictionary *dict=NULL)
            {
                const string _ = rx; return RegExp(_,dict);
            }
        };
    }
}

#endif

