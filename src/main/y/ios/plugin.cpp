
#include "y/ios/plugin.hpp"

namespace upsylon
{
    namespace ios
    {

        plugin:: plugin( const uint32_t u, const comm_mode m) throw() : counted_object(), uuid(u), mode(m)
        {
        }

        plugin:: ~plugin() throw()
        {
        }
        
    }

}

