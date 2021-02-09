
#include "y/gfx/crate.hpp"


namespace upsylon
{
    namespace GFX
    {

        unit_t Crate:: Items(const SubArea::Handle &handle) throw()
        {
            return handle.is_valid() ? handle->n : 0;
        }

        Crate:: Crate() throw()
        {
        }

        Crate:: ~Crate() throw()
        {
            
        }

    }

}


