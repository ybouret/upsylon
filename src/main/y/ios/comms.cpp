#include "y/ios/comms.hpp"

namespace upsylon {
    
    const char * comms:: shipping_style_id(const shipping_style m) throw()
    {
        switch (m) {
            case computed_block_size: return "constant_block_size";
            case flexible_block_size: return "flexible_block_size";
        }
        return "?";
    }
    
    const char * comms:: infrastructure_id(const infrastructure m) throw()
    {
        switch (m) {
            case homogeneous: return "homogeneous";
            case distributed: return "distributed";
        }
        return "?";
    }
    
    
}

