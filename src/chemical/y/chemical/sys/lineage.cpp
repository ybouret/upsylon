#include "y/chemical/sys/lineage.hpp"

namespace upsylon
{

    namespace Chemical
    {

        Lineage:: ~Lineage() throw()
        {
        }

        Lineage:: Lineage( const Species &sp ) :
        authority<const Species>(sp),
        bounded(true),
        primary(sp.rating,as_capacity)
        {

        }


        void Lineage:: link(const Primary::Pointer &p) throw()
        {
            aliasing::_(primary).push_back_(p);
            if(!p->bounded)
            {
                aliasing::_(bounded) = false;
            }
        }

    }

}
