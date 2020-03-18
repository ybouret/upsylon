//! \file

#ifndef Y_JARGON_TYPES_INCLUDED
#define Y_JARGON_TYPES_INCLUDED 1

#include "y/string.hpp"
#include "y/ptr/arc.hpp"

namespace upsylon {
 
    namespace Jargon {
     
        typedef object                Object;        //!< alias
        typedef counted_object        CountedObject; //!< alias
        typedef arc_ptr<const string> Tag;           //!< shared tag
    }
    
}

#endif

