

#include "y/chemical/sys/replica.hpp"

namespace upsylon
{
    namespace Chemical
    {

        Replica:: ~Replica() throw()
        {
        }

        Replica:: Replica(const Species     &sp,
                          const iAccessible &nu_,
                          const ENode       *en_) throw() :
        Object(),
        authority<const Species>(sp),
        nu(nu_),
        en(en_)
        {
            assert(en_);
        }

        void Replica:: fill( iAddressable &v ) const throw()
        {
            for(const ENode *node=en;node;node=node->next)
            {
                const Equilibrium &E = ***node;
                const size_t       i = E.indx;
                v[i] = nu[i];
            }
        }


    }

}
