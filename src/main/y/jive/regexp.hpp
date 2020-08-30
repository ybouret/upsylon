//! \file

#ifndef Y_JIVE_REGEXP_INCLUDED
#define Y_JIVE_REGEXP_INCLUDED 1

#include "y/jive/pattern.hpp"

namespace upsylon {
    
    namespace Jive
    {
        
        class Dictionary; //! < forward declaration
        
        //! expression to pattern
        Pattern * RegExp(const string &rx, const Dictionary *);
      
        //! expression to pattern, wrapper
        Pattern * RegExp(const char   *rx, const Dictionary *);
        
        
    }
    
}

#endif
