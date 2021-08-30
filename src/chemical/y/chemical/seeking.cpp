

#include "y/chemical/seeking.hpp"


namespace upsylon
{
    namespace Chemical
    {

        Seeking::~ Seeking() throw()
        {
        }

        Seeking:: Seeking(const Species &usr,
                          const iMatrix &NuT) :
        sp(usr),
        nu(NuT[usr.indx])
        {

        }

        
    }

}
