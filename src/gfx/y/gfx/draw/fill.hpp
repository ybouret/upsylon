//! \file

#ifndef Y_GRAPHIC_DRAW_FILL_INCLUDED
#define Y_GRAPHIC_DRAW_FILL_INCLUDED 1

#include "y/gfx/draw/clip.hpp"
#include "y/gfx/draw/putpixel.hpp"
#include "y/gfx/pixmap.hpp"

namespace upsylon {

    namespace graphic {

        namespace draw {

            //! fill an area by putPixel
            template <typename T, typename PUTPIXEL>
            inline void _fill(pixmap<T>    &img,
                              unit_t        x0,
                              unit_t        y0,
                              unit_t        x1,
                              unit_t        y1,
                              PUTPIXEL     &putPixel)
            {
                if(x1<x0) cswap(x1,x0);
                if(y1<y0) cswap(y1,y0);
                if( clip::accept(x0,y0,x1,y1,img) )
                {
                    assert(x0<=x1);
                    assert(y0<=y1);
                    for(unit_t y=y1;y>=y0;--y)
                    {
                        typename pixmap<T>::row &r = img[y];
                        for(unit_t x=x1;x>=x0;--x)
                        {
                            const coord p(x,y);
                            putPixel(r[x],p);
                        }
                    }
                }
            }



#define Y_FILL_ARGS pixmap<T> &pxm, const unit_t x0, const unit_t y0, const unit_t x1, const unit_t y1 //!< args for fill
#define Y_FILL_CALL _fill(pxm,x0,y0,x1,y1,proc)                                                        //!< call for fill

            Y_GFX_DRAW_IMPL(fill,Y_FILL_ARGS,Y_FILL_CALL)

#undef Y_FILL_CALL
#undef Y_FILL_ARGS

            
            
        }

    }
}

#endif

