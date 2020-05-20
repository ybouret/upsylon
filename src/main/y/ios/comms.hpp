//! \file

#ifndef Y_IOS_COMMS_INCLUDED
#define Y_IOS_COMMS_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon {
    
    
    //! information about comms
    struct comms
    {
        //! shipping method
        enum shipping_style
        {
            computed_block_size, //!< assuming host and peer know the block size
            flexible_block_size, //!< host and peer need to exchange the block size
        };
        
        //! commputing infrastucture
        enum infrastructure
        {
            homogeneous = 0x01, //!< same binary layouts
            distributed = 0x02  //!< binary layouts may change
        };
        
        static const char *shipping_style_id(const shipping_style) throw(); //!< get identifier for shipping_style
        static const char *infrastructure_id(const infrastructure) throw(); //!< get identifier for infrastructure


    };
    
    
}

#endif

