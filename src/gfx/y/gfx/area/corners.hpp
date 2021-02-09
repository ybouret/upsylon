//! \file

#ifndef Y_GFX_CORNERS_INCLUDED
#define Y_GFX_CORNERS_INCLUDED 1

#include "y/gfx/area/crate.hpp"

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
        class Corners : public virtual Crate
        {
        public:
            explicit Corners(const Area &area); //!< setup
            virtual ~Corners() throw();         //!< cleanup

            const SubArea::Handle bottom_left;  //!< corner at bottom|left
            const SubArea::Handle bottom_right; //!< corner at bottom|right
            const SubArea::Handle top_left;     //!< corner at top|left
            const SubArea::Handle top_right;    //!< corner at top|right
            const unit_t          on_vertex;    //!< 0,1,2,4

            //! display
            friend std::ostream & operator <<(std::ostream &, const Corners &);
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Corners);
        };
    }

}
#endif

