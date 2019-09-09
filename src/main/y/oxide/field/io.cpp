#include "y/oxide/field/io.hpp"
#include "y/exception.hpp"

namespace upsylon
{

    namespace Oxide
    {
        IO:: ~IO() throw()
        {
        }

#define Y_OXIDE_RAW(TYPE) declare<TYPE>( ios::plugin::save_raw<TYPE>, ios::plugin::
        IO:: IO() : plugins()
        {

        }

        void IO:: declare(const std::type_info        &t,
                          const ios::plugin::save_proc s,
                          const ios::plugin::load_proc l)
        {
            const Plugin *plg = plugins.search(t);
            if(plg)
            {
                const ios::plugin &p = **plg;
                if(p.save==s&&p.load==l)
                {
                    // ok, multiple type
                    return;
                }
                else
                {
                    throw exception("IO::declare(different plugins for same types!");
                }
            }
            else
            {
                const Plugin P = new _Plugin(t,s,l);
                if(!plugins.insert(P))
                {
                    throw exception("IO::declare(unexpected failure)");
                }
            }
        }
    }

}

