//! \file

#ifndef Y_INK_DRAW_LINE_INCLUDED
#define Y_INK_DRAW_LINE_INCLUDED 1

#include "y/ink/draw/putpixel.hpp"

namespace upsylon
{
    namespace Ink
    {
        namespace Draw
        {

#define Y_INK_DRAW_HLINE(X) const coord q(X,y0); proc(pxm,q)

            template <typename T,typename PROC>
            inline void __HLine(Pixmap<T>   &pxm,
                                const unit_t x0,
                                const unit_t y0,
                                const unit_t x1,
                                PROC        &proc)
            {
                assert(x0>=0);
                assert(x0<pxm.w);
                assert(x1>=x0);
                assert(x1<pxm.w);
                assert(y0>=0);
                assert(y0<pxm.h);
                Y_LOOP_FUNC_(x1-x0+1,Y_INK_DRAW_HLINE,x0);
            }

            template <typename T,typename PROC>
            inline void _HLine(Pixmap<T>   &pxm,
                               unit_t       x0,
                               unit_t       y0,
                               unit_t       x1,
                               PROC        &proc)
            {
                if(y0<0||y0>=pxm.h) return;
                if(x1<x0) cswap(x0,x1);
                if(x0>=pxm.w||x1<0) return;
                __HLine(pxm,max_of<unit_t>(x0,0),y0,min_of<unit_t>(x1,pxm.upper.x),proc);
            }

            template <typename T>
            inline void HLine(Pixmap<T>   &pxm,
                              unit_t       x0,
                              unit_t       y0,
                              unit_t       x1,
                              const T      color)
            {
                PutPixel::Copy<T> proc(color);
                _HLine(pxm,x0,y0,x1,proc);
            }

        }

    }
}

#endif

