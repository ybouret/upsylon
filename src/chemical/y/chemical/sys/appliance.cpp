
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
        nu(coef)
        {
        }

    }
}

namespace upsylon
{
    namespace Chemical
    {

        Consumer:: ~Consumer() throw()
        {
        }

        Consumer:: Consumer(const unit_t coef, const Primary &self) throw() :
        Appliance(coef,self)
        {
        }

    }
}

namespace upsylon
{
    namespace Chemical
    {

        Producer:: ~Producer() throw()
        {
        }

        Producer:: Producer(const unit_t coef, const Primary &self) throw() :
        Appliance(coef,self)
        {
        }

    }
}
