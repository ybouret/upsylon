
#include "y/alchemy/actor.hpp"
#include "y/type/utils.hpp"

namespace upsylon
{
    namespace Alchemy
    {
        Actor:: Actor(const Species &_sp, const unsigned long _nu) throw() :
        authority<const Species>(_sp),
        nu(_nu)
        {
        }

        Actor:: ~Actor() throw() {}

        Actor:: Actor(const Actor &other) throw() :
        authority<const Species>(other),
        nu(other.nu)
         {
        }

        const string & Actor:: key() const throw() { return (**this).name; }
        
        double Actor:: maxExtent(const double C) const throw()
        {
            return max_of<double>(C/nu,0);
        }

    }
}

