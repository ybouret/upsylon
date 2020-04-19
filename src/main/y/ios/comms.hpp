//! \file

#ifndef Y_IOS_COMMS_INCLUDED
#define Y_IOS_COMMS_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon {
    
    
    //! information about comms
    struct comms
    {
        enum delivery
        {
            computed_block_size, //!< assuming host and peer know the block size
            flexible_block_size, //!< host and peer need to exchange the block size
        };
        
        enum topology
        {
            homogeneous = 0x01, //!< same binary layouts
            distributed = 0x02  //!< binary layouts may change
        };
        
        static const char *delivery_id(const delivery) throw();
        static const char *topology_id(const topology) throw();
        
    };
    
    
}

#endif

