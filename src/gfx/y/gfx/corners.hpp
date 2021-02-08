//! \file

#ifndef Y_GFX_CORNERS_INCLUDED
#define Y_GFX_CORNERS_INCLUDED 1

#include "y/gfx/sub-area.hpp"

namespace upsylon
{
    namespace GFX
    {
        //______________________________________________________________________
        //
        //
        //! pre-computed corners
        //
        //______________________________________________________________________
        class Corners : public SubArea::Holder
        {
        public:
            explicit Corners(const Area &area); //!< setup
            virtual ~Corners() throw();         //!< cleanup

            const SubArea::Handle bottom_left;  //!< corner at bottom|left
            const SubArea::Handle bottom_right; //!< corner at bottom|right
            const SubArea::Handle top_left;     //!< corner at top|left
            const SubArea::Handle top_right;    //!< corner at top|right

            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Corners);
        };
    }

}
#endif

