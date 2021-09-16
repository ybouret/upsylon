
#include "y/chemical/sys/appliance.hpp"

namespace upsylon
{
    namespace Chemical
    {

        Appliance:: ~Appliance() throw()
        {
        }

        Appliance:: Appliance(const unit_t coef, const Primary &self) throw() :
        Object(),
        authority<const Primary>(self),
        dnode<Appliance>(),
        nu(coef)
        {
        }

    }
}

