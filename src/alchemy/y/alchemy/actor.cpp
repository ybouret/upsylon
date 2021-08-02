
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
            const double ans = max_of<double>(C/nu,0);
            //std::cerr << "(" << **this << "=" << C << ")/" << nu << " => " << ans <<  " @" << (**this).indx << std::endl;
            return ans;
        }

    }
}

