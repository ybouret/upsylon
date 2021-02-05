//! \file

#ifndef Y_OS_PAGE_SIZE_INCLUDED
#define Y_OS_PAGE_SIZE_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon
{


    //__________________________________________________________________________
    //
    //
    //! system page size
    //
    //__________________________________________________________________________
    struct page_size
    {
        static size_t get(); //!< query page size from system
    };

}


#endif
