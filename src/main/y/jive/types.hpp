//! \file

#ifndef Y_JIVE_TYPES_INCLUDED
#define Y_JIVE_TYPES_INCLUDED 1

#include "y/ptr/arc.hpp"
#include "y/string.hpp"

namespace upsylon {

    namespace Jive
    {

        typedef arc_ptr<const string> Tag;    //!< reusable tag
        typedef object                Object; //!< base class
    }

}

#endif
