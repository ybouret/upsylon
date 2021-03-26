
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

        //______________________________________________________________________
        //
        //! gradation to produce a color ramp
        //______________________________________________________________________
        class gradation : public color_ramp, public gradation_
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~gradation() throw();           //!< cleanup
            explicit gradation(const size_t n=0);   //!< setup

            //__________________________________________________________________
            //
            // interface
            //__________________________________________________________________
            virtual size_t      dimensions() const throw(); //!< size()
            virtual const rgba *repository() const throw(); //!< first color
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(gradation);
        };

    }

}

#endif
