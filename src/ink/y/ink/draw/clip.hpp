//! \file
#ifndef Y_INK_DRAW_CLIP_INCLUDED
#define Y_INK_DRAW_CLIP_INCLUDED 1

#include "y/ink/area.hpp"

namespace upsylon
{
    namespace Ink
    {

        namespace Draw
        {
            //! 2D clipping algorithm
            struct Clip
            {
                static const size_t Inside = 0x00; //!< inside status
                static const size_t Left   = 0x01; //!< left   status
                static const size_t Right  = 0x02; //!< Right  status
                static const size_t Bottom = 0x04; //!< Bottom status
                static const size_t Top    = 0x08; //!< Top    status

                //! compute the status of a given point
                static
                size_t StatusOf( const unit_t x, const unit_t y, const Area &area ) throw();

                //! compute the status of a given point
                static inline
                size_t StatusOf( const coord c, const Area &area) throw() { return StatusOf(c.x,c.y,area); }

                //! clip the segment, return true if something to draw
                static
                bool   Accept(unit_t &x0,
                              unit_t &y0,
                              unit_t &x1,
                              unit_t &y1,
                              const Area &area ) throw();
            };
            
        }
    }
}

#endif

