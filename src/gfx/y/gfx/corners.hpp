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

            const SubAreaHandle bottom_left;
            const SubAreaHandle bottom_right;
            const SubAreaHandle top_left;
            const SubAreaHandle top_right;

            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Corners);
        };
    }

}
#endif

