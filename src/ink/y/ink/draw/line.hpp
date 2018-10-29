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

            //! macro for hline loop
#define Y_INK_DRAW_HLINE(X) const coord q(X,y0); proc(pxm,q)

            //! clipped hline
            template <typename T,typename PROC>
            inline void __HLine(Pixmap<T>   &pxm,
                                const unit_t x0,
                                const unit_t y0,
                                const unit_t x1,
                                PROC        &proc)
            {
                assert( pxm.has( coord(x0,y0) ) );
                assert( pxm.has( coord(x1,y0) ) );
                assert(x0<=x1);
                Y_LOOP_FUNC_(x1-x0+1,Y_INK_DRAW_HLINE,x0);
            }
#undef Y_INK_DRAW_HLINE

            //! clipping hline
            template <typename T,typename PROC>
            inline void _HLine(Pixmap<T>   &pxm,
                               unit_t       x0,
                               unit_t       y0,
                               unit_t       x1,
                               PROC        &proc)
            {
                if(y0<0||y0>pxm.upper.y) return;
                if(x1<x0) cswap(x0,x1);
                if(x0>pxm.upper.x||x1<0) return;
                __HLine(pxm,max_of<unit_t>(x0,0),y0,min_of<unit_t>(x1,pxm.upper.x),proc);
            }

            //! colored hline
            template <typename T>
            inline void HLine(Pixmap<T>   &pxm,
                              const unit_t x0,
                              const unit_t y0,
                              const unit_t x1,
                              const T      color)
            {
                PutPixel::Copy<T> proc(color);
                _HLine(pxm,x0,y0,x1,proc);
            }

            //! blended colored hline
            template <typename T>
            inline void HLine(Pixmap<T>    &pxm,
                              unit_t        x0,
                              unit_t        y0,
                              unit_t        x1,
                              const T       color,
                              const uint8_t alpha)
            {
                PutPixel::Blend<T> proc(color,alpha);
                _HLine(pxm,x0,y0,x1,proc);
            }


            //! the Hline into mask
            template <typename T>
            inline void HLine(Pixmap<T>    &pxm,
                              unit_t        x0,
                              unit_t        y0,
                              unit_t        x1,
                              Mask         &mask)
            {
                PutPixel::Register proc(mask);
                _HLine(pxm,x0,y0,x1,proc);
            }

        }
    }
}

namespace upsylon
{
    namespace Ink
    {
        namespace Draw
        {
            //! macro for vline loop
#define Y_INK_DRAW_VLINE(Y) const coord q(x0,Y); proc(pxm,q)

            //! clipped vline
            template <typename T,typename PROC>
            inline void __VLine(Pixmap<T>   &pxm,
                                const unit_t x0,
                                const unit_t y0,
                                const unit_t y1,
                                PROC        &proc)
            {
                assert( pxm.has( coord(x0,y0) ) );
                assert( pxm.has( coord(x0,y1) ) );
                assert(y0<=y1);
                Y_LOOP_FUNC_(y1-y0+1,Y_INK_DRAW_VLINE,y0);
            }
#undef Y_INK_DRAW_VLINE

            //! clipping vline
            template <typename T,typename PROC>
            inline void _VLine(Pixmap<T>   &pxm,
                               unit_t       x0,
                               unit_t       y0,
                               unit_t       y1,
                               PROC        &proc)
            {
                if(x0<0||x0>pxm.upper.x) return;
                if(y1<y0) cswap(y0,y1);
                if(y0>pxm.upper.y||y1<0) return;
                __VLine(pxm,x0,max_of<unit_t>(y0,0), min_of<unit_t>(y1,pxm.upper.y), proc);
            }

            //! clipped vline
            template <typename T>
            inline void VLine(Pixmap<T>   &pxm,
                              const unit_t x0,
                              const unit_t y0,
                              const unit_t y1,
                              const T      color)
            {
                PutPixel::Copy<T> proc(color);
                _VLine(pxm,x0,y0,y1,proc);
            }

            //! clipped vline
            template <typename T>
            inline void VLine(Pixmap<T>    &pxm,
                              const unit_t  x0,
                              const unit_t  y0,
                              const unit_t  y1,
                              const T       color,
                              const uint8_t alpha)
            {
                PutPixel::Blend<T> proc(color,alpha);
                _VLine(pxm,x0,y0,y1,proc);
            }

            //! the VLine into mask
            template <typename T>
            inline void VLine(Pixmap<T>    &bmp,
                              const unit_t  x0,
                              const unit_t  y0,
                              const unit_t  y1,
                              Mask         &mask)
            {
                PutPixel::Register proc(mask);
                _Vline(bmp,x0,y0,y1,proc);
            }
        }

    }
}

#endif

