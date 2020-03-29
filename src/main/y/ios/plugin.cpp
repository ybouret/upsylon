
#include "y/ios/plugin.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace ios
    {

        plugin:: plugin( const uint32_t u, const comm_mode m, const char *id):
        counted_object(), uuid(u), mode(m), name(id)
        {
        }

        plugin:: ~plugin() throw()
        {
        }
        
        void plugin:: missing_data(const char *typeName) const
        {
            assert(typeName);
            throw exception("missing %s_plugin<%s> data", *name,typeName);
        }

        
    }

}

