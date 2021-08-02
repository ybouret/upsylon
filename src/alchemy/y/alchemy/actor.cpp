
#include "y/alchemy/actor.hpp"
#include "y/type/utils.hpp"
#include "y/core/ipower.hpp"
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
            return ans;
        }

        double Actor:: activity(const accessible<double> &C) const throw()
        {
            return ipower<unsigned long>(C[(**this).indx],nu);
        }

        double Actor:: activity(const accessible<double> &C, const double xi) const throw()
        {
            double              c = C[(**this).indx];
            const unsigned long n = nu;
            c += (n*xi);
            return  ipower<double>( max_of<double>(c,0), n);
        }


    }
}

