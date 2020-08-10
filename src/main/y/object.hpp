//! \file
#ifndef Y_OBJECT_INCLUDED
#define Y_OBJECT_INCLUDED 1

#include "y/memory/tight/object.hpp"
#include "y/object-parameters.hpp"


namespace upsylon
{
    //! specific tight object for upsylon
    typedef memory::tight::object<Y_CHUNK_SIZE,Y_LIMIT_SIZE> object;
}

#endif


