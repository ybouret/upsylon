
#include "y/ios/plugin.hpp"

namespace upsylon
{
    namespace ios
    {

        plugin:: plugin( const uint32_t u ) throw() : counted_object(), uuid(u)
        {
        }

        plugin:: ~plugin() throw()
        {
        }
        

        _plugin:: ~_plugin() throw()
        {
        }

        _plugin:: _plugin( const _plugin &_) throw() :
        save(_.save),
        load(_.load)
        {
        }

        _plugin:: _plugin(const save_proc on_save, const load_proc on_load ) throw() :
        save(on_save),
        load(on_load)
        {
        }
        

    }

}

