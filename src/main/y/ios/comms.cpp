#include "y/ios/comms.hpp"

namespace upsylon {
    
    const char * comms:: delivery_id(const delivery m) throw()
    {
        switch (m) {
            case computed_block_size: return "constant_block_size";
            case flexible_block_size: return "flexible_block_size";
        }
        return "?";
    }
    
    const char * comms:: topology_id(const topology m) throw()
    {
        switch (m) {
            case homogeneous: return "homogeneous";
            case distributed: return "distributed";
        }
        return "?";
    }
    
    
}

