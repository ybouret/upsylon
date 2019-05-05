//! \file
#ifndef Y_OBJECT_INCLUDED
#define Y_OBJECT_INCLUDED 1

#include "y/memory/small-object.hpp"
#include "y/object-parameters.hpp"


namespace upsylon
{
    //! specific small object for upsylon
    typedef memory::small_object<Y_LIMIT_SIZE,Y_CHUNK_SIZE> object;
}

#endif


