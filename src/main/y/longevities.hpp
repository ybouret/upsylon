//! \file
#ifndef Y_LONGEVITIES_INCLUDED
#define Y_LONGEVITIES_INCLUDED 1

#include "y/concurrent/at-exit.hpp"
#include "y/type/ints.hpp"

namespace upsylon
{
    //! table of global longevities
    struct longevity_for
    {
        typedef at_exit::longevity type;                     //!< alias
        static const type          maximum = limit_of<type>::maximum; //!< maximum integral type

        static const type memory_global = maximum;
        static const type memory_object = memory_global-1;
        static const type memory_pooled = memory_object-1;
        static const type system_env    = memory_pooled-1;
        static const type vfs_local     = system_env-1;
        
    };
}

#endif

