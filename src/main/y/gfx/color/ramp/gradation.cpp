
#include "y/gfx/color/ramp/gradation.hpp"

namespace upsylon
{

    namespace graphic
    {

        color_gradation:: ~color_gradation() throw()
        {
        }

        color_gradation:: color_gradation() throw() : color_ramp_(), color_gradation_()
        {
        }

        const color_repository & color_gradation:: repository() const throw()
        {
            return *this;
        }

    }

}

