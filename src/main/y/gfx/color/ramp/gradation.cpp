
#include "y/gfx/color/ramp/gradation.hpp"

namespace upsylon
{

    namespace graphic
    {

        gradation:: ~gradation() throw()
        {
        }

        gradation:: gradation() throw() : color_ramp_(), gradation_()
        {
        }

        const color_repository & gradation:: repository() const throw()
        {
            return *this;
        }

    }

}

