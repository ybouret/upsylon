//! \file
#ifndef Y_LANG_PATTERN_COMPILER_INCLUDED
#define Y_LANG_PATTERN_COMPILER_INCLUDED 1

#include "y/lang/pattern.hpp"

namespace upsylon
{
    namespace Lang
    {
        class Dictionary;
        class RXCompiler; //!< forward declaration to avoid class collision

        //! compile a regular expression
        Pattern *RegExp( const string &rx, const Dictionary *dict=NULL);

        //! compile a regular expression, wrapper
        Pattern *RegExp(const char *rx, const Dictionary *dict=NULL);

        //! convert a c string to a regular expression
        string StringToRegExp(const char *text, const size_t size);

        //! convert a c string to a regular expression
        string StringToRegExp(const char *text);

        //! convert a c char to a regular expression
        string StringToRegExp(const char C );

        //! convert a string to a regular expression
        string StringToRegExp(const string &s);
    }
}

#endif

