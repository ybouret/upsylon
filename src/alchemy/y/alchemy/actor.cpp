
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

        const string & actor:: key() const throw() { return (**this).name; }






    }
}

#include "y/alchemy/library.hpp"
#include <iomanip>

namespace upsylon
{
    namespace alchemy
    {
        std::ostream & actor:: display(std::ostream  &os,
                                       const library &lib) const
        {

            if(nu>1)
            {
                os << nu << ' ';
            }
            lib.prefix(os,**this);
            return os;
        }

    }
}

