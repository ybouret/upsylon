//! \file
#ifndef Y_OBJECT_INCLUDED
#define Y_OBJECT_INCLUDED 1

#include "y/memory/small-object.hpp"

#ifndef Y_LIMIT_SIZE
#define Y_LIMIT_SIZE 256
#endif

#ifndef Y_CHUNK_SIZE
#define Y_CHUNK_SIZE 4096
#endif

namespace upsylon
{
    typedef memory::small_object<Y_LIMIT_SIZE,Y_CHUNK_SIZE> object;
}

#endif


