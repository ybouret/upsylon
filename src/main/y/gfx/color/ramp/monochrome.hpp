//! \file

#ifndef Y_GFX_COLOR_RAMP_MONOCHROME_INCLUDED
#define Y_GFX_COLOR_RAMP_MONOCHROME_INCLUDED 1

#include "y/gfx/color/ramp/tableau.hpp"

namespace upsylon
{

    namespace graphic
    {
        //______________________________________________________________________
        //
        //
        //! create a monochrome repository
        //
        //______________________________________________________________________
        class monochrome
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            monochrome(const rgba &C) throw();           //!< setup color
            ~monochrome() throw();                       //!< cleanup
            monochrome(const monochrome &other) throw(); //!< no-throw copy

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            static const size_t size = 2;     //!< black+color
            const rgba          data[size];   //!< black->color

        private:
            Y_DISABLE_ASSIGN(monochrome);
        };
    }
}

#endif

