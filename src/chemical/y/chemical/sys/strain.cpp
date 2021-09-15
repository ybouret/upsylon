
#include "y/chemical/sys/strain.hpp"

namespace upsylon
{
    namespace Chemical
    {

        Strain:: ~Strain() throw()
        {

        }

        Strain:: Strain(const Species &sp) :
        Object(),
        authority<const Species>(sp),
        Flow(Bounded),
        primary( (**this).rating, as_capacity)
        {
            
        }

        void  Strain:: link(const Primary::Pointer &p) throw()
        {
            aliasing::_(primary).push_back_(p);

            if( (**p).state == Endless )
            {
                aliasing::_(state) = Endless;
            }
        }


    }

}

