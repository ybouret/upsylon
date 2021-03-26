

//! \file

#ifndef Y_GFX_COLOR_RAMP_TABLEAU_INCLUDED
#define Y_GFX_COLOR_RAMP_TABLEAU_INCLUDED 1

#include "y/gfx/color/ramp.hpp"

namespace upsylon
{

    namespace graphic
    {

        template <typename CLASS>
        class tableau : public color_ramp
        {
        public:
            explicit tableau(const CLASS &) throw()
            {
            }

            virtual ~tableau() throw()
            {
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(tableau);
        };

    }

}

