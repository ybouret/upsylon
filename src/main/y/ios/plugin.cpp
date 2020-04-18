
#include "y/ios/plugin.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace ios
    {

        plugin:: plugin( const uint32_t u, const comms::manner m) throw():
        counted_object(), uuid(u), mode(m)
        {
        }

        plugin:: ~plugin() throw()
        {
        }
        
        void plugin:: missing_data(const char *pluginName, const char *typeName) const
        {
            assert(typeName);
            assert(pluginName);
            throw exception("missing %s_plugin<%s> data", pluginName,typeName);
        }

        
    }

}

