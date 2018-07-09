//! \file
#ifndef Y_MEMORY_OWNERSHIP_INCLUDED
#define Y_MEMORY_OWNERSHIP_INCLUDED 1

namespace upsylon
{
    namespace memory
    {
        //! detect address ownership
        enum ownership
        {
            owned_by_prev, //!< in a chunk with lower memory range
            owned_by_this, //!< this chunk!
            owned_by_next  //!< in a chunk with higher memory range
        };

    }
}

#endif
