
//! \file

#ifndef Y_GFX_AREA_INCLUDED
#define Y_GFX_AREA_INCLUDED 1

#include "y/gfx/types.hpp"


namespace upsylon
{
    namespace GFX
    {
        //______________________________________________________________________
        //
        //
        //! basic area
        //
        //______________________________________________________________________
        class Area
        {
        public:
            const unit_t x; //!< bottom-left  position
            const unit_t y; //!< bottom-right position
            const unit_t w; //!< width  >= 0
            const unit_t h; //!< height >= 0
            const unit_t n; //!< width*height >= 0

            //! setup
            Area(const unit_t X, const unit_t Y, const unit_t W, const unit_t H);
            //! copy
            Area(const Area &) throw();
            //! cleanup
            virtual ~Area() throw();
            

        private:
            Y_DISABLE_ASSIGN(Area);
        };
    }

}

#endif

