
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
        typedef vector<rgba,memory::dyadic> gradation_;

        class gradation : public color_ramp_, public gradation_
        {
        public:
            virtual ~gradation() throw();
            explicit gradation() throw();
            virtual const color_repository & repository() const throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(gradation);
        };


    }

}

#endif
