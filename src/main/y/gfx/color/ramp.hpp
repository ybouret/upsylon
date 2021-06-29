//! \file

#ifndef Y_GFX_COLOR_RAMP_INCLUDED
#define Y_GFX_COLOR_RAMP_INCLUDED 1

#include "y/gfx/color/type-to-rgba.hpp"
#include "y/sequence/accessible.hpp"

namespace upsylon
{

    namespace graphic
    {

        class color_ramp
        {
        public:
            
            static rgba build(const float u, const accessible<rgba> &arr) throw();


        private:
            Y_DISABLE_COPY_AND_ASSIGN(color_ramp);
        };



    }

}

#endif

