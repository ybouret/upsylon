//! \file

#ifndef Y_JIVE_TYPES_INCLUDED
#define Y_JIVE_TYPES_INCLUDED 1

#include "y/ptr/arc.hpp"
#include "y/string.hpp"

namespace upsylon {

    namespace Jive
    {

        typedef object                Object;
        typedef arc_ptr<const string> Tag;
        
    }

}

#endif
