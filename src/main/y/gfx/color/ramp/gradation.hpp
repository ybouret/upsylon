
//! \file

#ifndef Y_GFX_COLOR_RAMP_GRADATION_INCLUDED
#define Y_GFX_COLOR_RAMP_GRADATION_INCLUDED 1

#include "y/gfx/color/ramp.hpp"
#include "y/sequence/vector.hpp"
#include "y/memory/allocator/dyadic.hpp"

namespace upsylon
{

    namespace graphic
    {

        //______________________________________________________________________
        //
        //! base class for a dynamic gradation
        //______________________________________________________________________
        typedef vector<rgba,memory::dyadic> color_gradation_;


        //______________________________________________________________________
        //
        //
        //! dynamic gradation
        //
        //______________________________________________________________________
        class color_gradation : public color_ramp_, public color_gradation_
        {
        public:
            virtual ~color_gradation() throw(); //!< cleanup
            explicit color_gradation() throw(); //!< setup
            virtual const color_repository & repository() const throw(); //!< self

        private:
            Y_DISABLE_COPY_AND_ASSIGN(color_gradation);
        };


    }

}

#endif
