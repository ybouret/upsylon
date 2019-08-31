
#include "y/ios/plugin.hpp"

namespace upsylon
{
    namespace ios
    {

        plugin:: ~plugin() throw()
        {
        }

        plugin:: plugin( const plugin &_) throw() :
        save(_.save),
        load(_.load)
        {
        }

        plugin:: plugin(const save_proc on_save, const load_proc on_load ) throw() :
        save(on_save),
        load(on_load)
        {
        }
        

    }

}

