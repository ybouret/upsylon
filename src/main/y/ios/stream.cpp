
#include "y/ios/stream.hpp"

namespace upsylon
{
    
    const char * comms:: manner_text(const manner m) throw()
    {
        switch (m) {
            case constant: return "constant";
            case variable: return "variable";
        }
        return "?";
    }
    
    const char * comms:: medium_text(const medium m) throw()
    {
        switch (m) {
            case homogeneous: return "homogeneous";
            case distributed: return "distributed";
        }
        return "?";
    }
    
    namespace ios
    {
        stream:: ~stream() throw() {}

        stream:: stream() throw() {}
    }
}
