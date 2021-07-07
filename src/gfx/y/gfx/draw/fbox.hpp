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
                    switch(y1-y0)
                    {
                        case 0:
                            if(clip::accept(x0,y0,x1,y1,pxm))
                                __hline(pxm,x0,y0,x1-x0+1,putPixel);
                            break;

                        default:
                        {
                            const unit_t yini=y0+1;
                            const unit_t yend=y1-1;
                            _vline(pxm,x0,yini,yend,putPixel);
                            _vline(pxm,x1,yini,yend,putPixel);
                        } /* FALLTHRU */
                        case 1:
                            _hline(pxm, x0, y0, x1,putPixel);
                            _hline(pxm, x0, y1, x1,putPixel);
                            break;
                    }
                }

            }



#define Y_FBOX_ARGS pixmap<T> &pxm, const unit_t x0, const unit_t y0, const unit_t x1, const unit_t y1 //!< args for fbox
#define Y_FBOX_CALL _fbox(pxm,x0,y0,x1,y1,proc)                                                        //!< call for fbox

            Y_GFX_DRAW_IMPL(fbox,Y_FBOX_ARGS,Y_FBOX_CALL)

#undef Y_FBOX_CALL
#undef Y_FBOX_ARGS


#define Y_FBOX_ARGS pixmap<T> &pxm, const area &a                             //!< args for fbox with area
#define Y_FBOX_CALL _fbox(pxm,a.lower.x,a.lower.y,a.upper.x,a.upper.y,proc)   //!< call for fbox with area

            Y_GFX_DRAW_IMPL(fbox,Y_FBOX_ARGS,Y_FBOX_CALL)

#undef Y_FBOX_CALL
#undef Y_FBOX_ARGS
            
        }

    }
}

#endif

