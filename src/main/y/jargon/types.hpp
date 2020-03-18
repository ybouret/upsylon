//! \file

#ifndef Y_JARGON_TYPES_INCLUDED
#define Y_JARGON_TYPES_INCLUDED 1

#include "y/ios/serializable.hpp"
#include "y/ios/tools/vizible.hpp"
#include "y/string.hpp"
#include "y/ptr/arc.hpp"

namespace upsylon {
 
    namespace Jargon {
     
        typedef object                Object;        //!< alias
        typedef counted_object        CountedObject; //!< alias
        typedef arc_ptr<const string> Tag;           //!< shared tag
        typedef ios::serializable     Serializable;  //!< alias
        typedef ios::vizible          Vizible;       //!< alias
    }
    
}

#endif

