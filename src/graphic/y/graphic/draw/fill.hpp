//! \file

#ifndef Y_GRAPHIC_DRAW_FILL_INCLUDED
#define Y_GRAPHIC_DRAW_FILL_INCLUDED 1

#include "y/graphic/draw/clip.hpp"
#include "y/graphic/draw/putpixel.hpp"

namespace upsylon {

    namespace Graphic {

        namespace Draw {

            //! fill an area by putPixel
            template <typename T, typename PUTPIXEL>
            inline void _Fill(Pixmap<T>    &img,
                              unit_t        x0,
                              unit_t        y0,
                              unit_t        x1,
                              unit_t        y1,
                              PUTPIXEL     &putPixel)
            {
                if(x1<x0) cswap(x1,x0);
                if(y1<y0) cswap(y1,y0);
                if( Clip::Accept(x0,y0,x1,y1,*img) )
                {
                    assert(x0<=x1);
                    assert(y0<=y1);
                    for(unit_t y=y1;y>=y0;--y)
                    {
                        typename Pixmap<T>::RowType &r = img[y];
                        for(unit_t x=x1;x>=x0;--x)
                        {
                            const Point p(x,y);
                            putPixel(r[x],p);
                        }
                    }
                }
            }

            //! fill with a solid color
            template <typename T>
            inline void Fill(Pixmap<T>    &img,
                             const unit_t x0,
                             const unit_t y0,
                             const unit_t x1,
                             const unit_t y1,
                             typename Pixmap<T>::param_type color)
            {
                PutPixel::Copy<T> proc(color);
                _Fill(img,x0,y0,x1,y1,proc);
            }


            //! fill with a given color/alpha
            template <typename T>
            inline void Fill(Pixmap<T>    &img,
                             const unit_t x0,
                             const unit_t y0,
                             const unit_t x1,
                             const unit_t y1,
                             typename Pixmap<T>::param_type color,
                             const uint8_t                  alpha)
            {
                PutPixel::Blend<T> proc(color,alpha);
                _Fill(img,x0,y0,x1,y1,proc);
            }

            //! gather mask
            template <typename T>
            inline void Fill(Pixmap<T>    &img,
                             const unit_t x0,
                             const unit_t y0,
                             const unit_t x1,
                             const unit_t y1,
                             Mask        &mask)
            {
                PutPixel::ToMask<T> proc(mask);
                _Fill(img,x0,y0,x1,y1,proc);
            }
            
        }

    }
}

#endif

