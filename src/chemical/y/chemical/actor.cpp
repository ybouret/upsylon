
#include "y/chemical/actor.hpp"

namespace upsylon
{
    namespace Chemical
    {
        Actor:: ~Actor() throw()
        {
            aliasing::_(nu)  = 0;
            aliasing::_(nu1) = 0;
            aliasing::_(snu) = 0;
        }
        
        Actor:: Actor(const Actor &a) throw() :
        sp(a.sp),
        nu(a.nu),
        nu1(a.nu1),
        snu(a.snu)
        {
        }
        
        Actor::Actor(const size_t n, const Species &s) throw() :
        sp(s),
        nu(n),
        nu1(nu-1),
        snu( unit_t(nu) )
        {
            assert(nu>0);
        }
        
        
    }
    
}

        
