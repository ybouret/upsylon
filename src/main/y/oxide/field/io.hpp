//! \file
#ifndef Y_OXIDE_FIELD_IO_INCLUDED
#define Y_OXIDE_FIELD_IO_INCLUDED 1

#include "y/oxide/types.hpp"
#include "y/ios/plugin.hpp"
#include "y/ios/ovstream.hpp"

namespace upsylon
{
    namespace Oxide
    {
        //! I/O on objects in fields
        struct IO
        {
            //! alias for block of data
            typedef ios::ovstream Block;
        };

    }
}

#endif

