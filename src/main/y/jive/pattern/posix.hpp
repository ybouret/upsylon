//! \file

#ifndef Y_JIVE_PATTERN_POSIX_INCLUDED
#define Y_JIVE_PATTERN_POSIX_INCLUDED 1

#include "y/jive/pattern.hpp"

namespace upsylon {

    namespace Jive {

        //! posix patterns
        struct posix
        {
            //------------------------------------------------------------------
            //
            // standard
            //
            //------------------------------------------------------------------
            static Pattern *lower();  //!< \verbatim[a-z]\endverbatim
            static Pattern *upper();  //!< \verbatim[A-Z]\endverbatim
            static Pattern *alpha();  //!< \verbatim[a-zA-Z]\endverbatim
            static Pattern *digit();  //!< \verbatim[0-9]\endverbatim
            static Pattern *alnum();  //!< \verbatim[a-zA-Z0-9]\endverbatim
            static Pattern *xdigit(); //!< \verbatim[0-9a-fA-F]\endverbatim
            static Pattern *blank();  //!< \verbatim[ \t]\endverbatim
            static Pattern *space();  //!< \verbatim[ \n\r\t\v\f]\endverbatim
            static Pattern *punct();  //!< \verbatim][!"#$%&'()*+,./:;<=>?@\^_`{|}~-\endverbatim

            //------------------------------------------------------------------
            //
            // extended
            //
            //------------------------------------------------------------------
            static Pattern *word();      //!< \verbatim[a-zA-Z0-9_]\endverbatim
            static Pattern *endl();      //!< \verbatim\n|\r|\r\n\endverbatim
            static Pattern *dot();       //!< \verbatim^endl\endverbatim
            static Pattern *core();      //!< core 7-bits string content, minus quote, dquote  and backslash
            static Pattern *vowel();     //!< vowels
            static Pattern *consonant(); //!< consonants

            //------------------------------------------------------------------
            //
            // query
            //
            //------------------------------------------------------------------
            static bool     exists(const string &id) throw(); //!< query internal database
            static bool     exists(const char   *id) throw(); //!< query internal database
            static Pattern *create(const string &id);         //!< create a new pattern
            static Pattern *create(const char   *id);         //!< create a new pattern
            static Pattern *query(const string &id);          //!< try to create a new pattern
            static Pattern *query(const char   *id);          //!< try to create a new pattern
            static const Vizible &root();                     //!< root node of internal database
        };

    }

}


#endif

