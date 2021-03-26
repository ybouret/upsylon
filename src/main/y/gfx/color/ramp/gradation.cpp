
#include "y/gfx/color/ramp/gradation.hpp"

namespace upsylon
{

    namespace graphic
    {

        gradation:: ~gradation() throw() {}

        gradation:: gradation(const size_t n) : color_ramp(), gradation_(n,as_capacity)
        {}

        size_t       gradation:: dimensions() const throw() { return size();   }
        const rgba * gradation:: repository() const throw() { return *(*this); }


    }

}

