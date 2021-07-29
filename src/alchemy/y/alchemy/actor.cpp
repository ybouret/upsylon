
#include "y/alchemy/actor.hpp"

namespace upsylon
{
    namespace alchemy
    {
        actor:: actor(const species &_sp, const unsigned _nu) throw() :
        authority<const species>(_sp),
        nu(_nu)
        {
        }

        actor:: ~actor() throw() {}

        actor:: actor(const actor &other) throw() :
        authority<const species>(other),
        nu(other.nu)
        {
        }

    }

}

