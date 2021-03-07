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
            segment(const coord,const unit_t) throw(); //!< setup
            ~segment() throw();                        //!< cleanup
            segment(const segment &) throw();          //!< copy
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const coord  origin; //!< where to start
            const unit_t length; //!< how may pixels
            
        private:
            Y_DISABLE_ASSIGN(segment);
        };
    }
}

#endif

