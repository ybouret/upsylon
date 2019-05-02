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
        typedef at_exit::longevity type;                              //!< alias
        static const type          maximum = limit_of<type>::maximum; //!< maximum integral type

        static const type memory_global = maximum;          //!< for global memory
        static const type memory_object = memory_global-1;  //!< small objects
        static const type memory_pooled = memory_object-1;  //!< pooled memory
        static const type system_env    = memory_pooled-1;  //!< environmnent API
        static const type vfs_local     = system_env-1;     //!< local file system API
        
    };
}

#endif

