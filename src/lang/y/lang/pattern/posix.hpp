//! \file
#ifndef Y_LANG_PATTERN_POSIX_INCLUDED
#define Y_LANG_PATTERN_POSIX_INCLUDED 1

#include "y/lang/pattern.hpp"

namespace upsylon
{
    namespace Lang
    {
        struct Posix
        {
            static Pattern *lower();  //!< [a-z]
            static Pattern *upper();  //!< [A-Z]
            static Pattern *alpha();  //!< [a-zA-Z]
            static Pattern *digit();  //!< [0-9]
            static Pattern *alnum();  //!< [a-zA-Z0-9]
            static Pattern *xdigit(); //!< [0-9a-fA-F]
            static Pattern *blank();  //!< [ \t]
            static Pattern *space();  //!< [ \n\r\t\v\f]


            static Pattern *word();   //!< [a-zA-Z0-9_]
            static Pattern *endl();   //!< \n|\r|\r\n
            static Pattern *dot();    //!< ^endl
        };
    }

}

#endif
