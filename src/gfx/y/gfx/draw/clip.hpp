//! \file

#ifndef Y_GRAPHIC_DRAW_CLIP
#define Y_GRAPHIC_DRAW_CLIP 1

#include "y/gfx/area.hpp"

namespace  upsylon {

    namespace graphic {

        namespace draw {

            //! 2D clipping algorithm
            struct clip
            {
                static const size_t Inside = 0x00; //!< inside status
                static const size_t Left   = 0x01; //!< left   status
                static const size_t Right  = 0x02; //!< Right  status
                static const size_t Bottom = 0x04; //!< Bottom status
                static const size_t Top    = 0x08; //!< Top    status

                //! compute the status of a given point
                static
                size_t status_of( const unit_t x, const unit_t y, const area &a ) throw();

                //! compute the status of a given point
                static inline
                size_t status_of( const coord c, const area &a) throw() { return status_of(c.x,c.y,a); }

                //! clip the segment, return true if something to draw
                static bool   accept(unit_t     &x0,
                                     unit_t     &y0,
                                     unit_t     &x1,
                                     unit_t     &y1,
                                     const area &a ) throw();
            };

        }

    }
}

#endif

