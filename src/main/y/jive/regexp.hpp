//! \file

#ifndef Y_JIVE_REGEXP_INCLUDED
#define Y_JIVE_REGEXP_INCLUDED 1

#include "y/jive/pattern.hpp"

namespace upsylon {
    
    namespace Jive
    {
        
        class Dictionary; //! < forward declaration
        
        //! expression to pattern
        /**
         - special: '.'=[:dot:]
         - jokers :
         -- '*'=zero or more
         -- '+'=one or more
         -- '?'=optional
         -- '~'=none
         -- '&'=ignore case
         */
        Pattern * RegExp(const string &rx, const Dictionary *);
      
        //! expression to pattern, wrapper
        Pattern * RegExp(const char   *rx, const Dictionary *);

        //! single char
        Pattern * RegExp(const char    C, const Dictionary *);
        
        //! compiler verbosity, to debug
        bool & RegExpVerbose() throw();
        
    }
    
}

#endif
