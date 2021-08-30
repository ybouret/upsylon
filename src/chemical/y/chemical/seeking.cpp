

#include "y/chemical/seeking.hpp"


namespace upsylon
{
    namespace Chemical
    {

        Seeking::~ Seeking() throw()
        {
        }

        Seeking:: Seeking(const Species &usr,
                          const iMatrix &NuT,
                          const ENode   *ptr) :
        sp(usr),
        nu(NuT[usr.indx]),
        en(0)
        {
            assert(NULL!=ptr);
            while(!nu[ (***ptr).indx])
            {
                ptr=ptr->next;
                assert(ptr);
            }
            aliasing::_(en)=ptr;
        }

        
    }

}
