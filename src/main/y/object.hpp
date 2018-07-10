//! \file
#ifndef Y_OBJECT_INCLUDED
#define Y_OBJECT_INCLUDED 1

#include "y/memory/small-object.hpp"

//! default limit size for small object
#ifndef Y_LIMIT_SIZE
#define Y_LIMIT_SIZE 256
#endif

//! default chunk size for small object, should be a power of two
#ifndef Y_CHUNK_SIZE
#define Y_CHUNK_SIZE 4096
#endif

namespace upsylon
{
    //! specific small object for upsylon
    typedef memory::small_object<Y_LIMIT_SIZE,Y_CHUNK_SIZE> object;
}

#endif


