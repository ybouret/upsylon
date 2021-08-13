
#include "y/alchemy/actor.hpp"
#include "y/type/utils.hpp"
#include "y/core/ipower.hpp"
#include "y/type/utils.hpp"

namespace upsylon
{
    namespace Alchemy
    {
        Actor:: Actor(const Species &_sp, const size_t _nu) throw() :
        authority<const Species>(_sp),
        nu(_nu),
        nu1(nu-1)
        {
        }

        Actor:: ~Actor() throw() {}

        Actor:: Actor(const Actor &other) throw() :
        authority<const Species>(other),
        nu(other.nu),
        nu1(other.nu1)
        {
        }

        const string & Actor:: key() const throw() { return (**this).name; }
        
        double Actor:: maxExtent(const double C) const throw()
        {
            const double ans = max_of<double>(C/nu,0);
            return ans;
        }

        double Actor:: activity(const Accessible &C) const throw()
        {
            return ipower<double>(C[(**this).indx],nu);
        }

        double Actor:: jacobian(const Accessible &C) const throw()
        {
            return nu * ipower<double>(C[(**this).indx],nu1);
        }


        double Actor:: activity(const Accessible &C, const double xi) const throw()
        {
            double              c = C[(**this).indx];
            const unsigned long n = nu;
            c += (n*xi);
            return  ipower<double>( max_of<double>(c,0), n);
        }

        long Actor:: z() const throw()
        {
            return long(nu) * ( (**this).z );
        }



    }
}

