//! \file

#ifndef Y_IOS_COMMS_INCLUDED
#define Y_IOS_COMMS_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon {
    
    
    //! information about comms
    struct comms
    {
        enum manner
        {
            constant, //!< assuming host and peer know the size
            variable  //!< the size must be exchanged
        };
        
        enum medium
        {
            homogeneous = 0x01, //!< same binary layouts
            distributed = 0x02  //!< binary layouts may change
        };
        
        static const char *manner_text(const manner) throw();
        static const char *medium_text(const medium) throw();
        
    };
    
    
}

#endif

