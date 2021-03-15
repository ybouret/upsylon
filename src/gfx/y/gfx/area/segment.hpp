//! \file

#ifndef Y_GFX_AREA_SEGMENT_INCLUDED
#define Y_GFX_AREA_SEGMENT_INCLUDED 1


#include "y/gfx/types.hpp"

namespace upsylon
{
    namespace graphic
    {
        //! horizontal segment of a tile
        class segment
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            segment(const unit_t Xmin, const unit_t Y, const unit_t Xmax) throw(); //!< setup
            ~segment() throw();                        //!< cleanup
            segment(const segment &) throw();          //!< copy

            coord head() const throw(); //!< (xmin,y)
            coord tail() const throw(); //!< (xmax,y)

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const unit_t xmin; //!< origin x
            const unit_t y;    //!< origin y
            const unit_t xmax; //!< final x
            const unit_t w;    //!< width

        private:
            Y_DISABLE_ASSIGN(segment);
        };
    }
}

#endif

