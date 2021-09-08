

#include "y/chemical/sys/replica.hpp"


namespace upsylon
{
    namespace Chemical
    {
        
        Replica:: ~Replica() throw()
        {
        }
        
        Replica:: Replica(const Species     &sp_,
                          const iAccessible &nu_,
                          const ENode       *en_) throw() :
        Object(),
        authority<const Species>(sp_),
        nu(nu_),
        en(en_)
        {
            assert(en);
            assert(nu[(***en).indx]);
        }
        
        void Replica:: activate( addressable<bool> &go ) const throw()
        {
            const ENode *node = en;
            go[ (***node).indx ] = true;
            for(node=node->next;node;node=node->next)
            {
                const size_t i = (***node).indx;
                if(nu[i]) go[i] = true;
            }
        }

    }
    
}

