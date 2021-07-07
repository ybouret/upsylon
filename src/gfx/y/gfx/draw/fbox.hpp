//! \file

#ifndef Y_GRAPHIC_DRAW_FBOX_INCLUDED
#define Y_GRAPHIC_DRAW_FBOX_INCLUDED 1

#include "y/gfx/draw/line.hpp"

namespace upsylon {

    namespace graphic {

        namespace draw {

            //! fbox using putpixel
            template <typename T, typename PUTPIXEL>
            inline void _fbox(pixmap<T>    &pxm,
                              unit_t        x0,
                              unit_t        y0,
                              unit_t        x1,
                              unit_t        y1,
                              PUTPIXEL     &putPixel)
            {
                if(x1<x0) cswap(x1,x0);
                if(y1<y0) cswap(y1,y0);

                if(x0>=x1)
                {
                    // width=1 => vertical line
                    if(clip::accept(x0,y0,x1,y1,pxm))
                    {
                        __vline(pxm,x0,y0,y1-y0+1,putPixel);
                    }
                }
                else
                {
                    assert(x0<x1);
                    if(y0>=y1)
                    {
                        // height=1 => horizontal line
                        if(clip::accept(x0,y0,x1,y1,pxm))
                        {
                            __hline(pxm,x0,y0,x1-x0+1,putPixel);
                        }
                    }
                    else
                    {
                        assert(y0<y1);
                        // full box
                    }
                }
                
            }



#define Y_FBOX_ARGS pixmap<T> &pxm, const unit_t x0, const unit_t y0, const unit_t x1, const unit_t y1 //!< args for fbox
#define Y_FBOX_CALL _fbox(pxm,x0,y0,x1,y1,proc)                                                        //!< call for fbox

            Y_GFX_DRAW_IMPL(fbox,Y_FBOX_ARGS,Y_FBOX_CALL)

#undef Y_FILL_CALL
#undef Y_FBOX_ARGS

            
            
        }

    }
}

#endif

