
//! \file

#ifndef Y_GRAPHIC_DRAW_LINE
#define Y_GRAPHIC_DRAW_LINE 1

#include "y/graphic/draw/clip.hpp"
#include "y/graphic/draw/putpixel.hpp"

namespace  upsylon {

    namespace Graphic {

        namespace Draw {

            //! draw an unsage Horizontal Line
            template <typename T, typename PUTPIXEL> inline
            void __HLine(Pixmap<T>   &pxm,
                         const unit_t x0,
                         const unit_t y0,
                         const size_t w,
                         PUTPIXEL    &putPixel)
            {
                typename Pixmap<T>::RowType &row = pxm[y0];
                for(size_t i=0;i<w;++i)
                {
                    putPixel( row[x0+i] );
                }
            }

            //! apply putpixel on a clipped horizontal line
            template <typename T, typename PUTPIXEL> inline
            void _HLine(Pixmap<T>   &pxm,
                        unit_t       x0,
                        unit_t       y0,
                        unit_t       x1,
                        PUTPIXEL    &putPixel)
            {
                unit_t y1 = y0;
                if(x1<x0) cswap(x1,x0);
                if( Clip::Accept(x0, y0, x1, y1, *pxm) )
                {
                    assert(x0<=x1);
                    __HLine(pxm,x0,y0,x1-x0+1,putPixel);
                }
            }


            //! set solid color on horizontal line
            template <typename T>
            void HLine(Pixmap<T>   &pxm,
                       unit_t       x0,
                       unit_t       y0,
                       unit_t       x1,
                       typename Pixmap<T>::param_type C)
            {
                PutPixel::Copy<T> cb(C);
                _HLine(pxm, x0, y0, x1,cb);
            }

            //! set transparent color on horizontal line
            template <typename T>
            void HLine(Pixmap<T>   &pxm,
                       unit_t       x0,
                       unit_t       y0,
                       unit_t       x1,
                       typename Pixmap<T>::param_type C,
                       const uint8_t                  alpha)
            {
                PutPixel::Blend<T> cb(C,alpha);
                _HLine(pxm, x0, y0, x1,cb);
            }


        }

        namespace Draw {

            //! unsafe vertical line
            template <typename T, typename PUTPIXEL> inline
            void __VLine(Pixmap<T>   &pxm,
                         const unit_t x0,
                         const unit_t y0,
                         const size_t h,
                         PUTPIXEL    &putPixel)
            {
                for(size_t j=0;j<h;++j)
                {
                    putPixel( pxm[y0+j][x0] );
                }
            }

            //! apply putpixel on a clipped vertical line
            template <typename T, typename PUTPIXEL> inline
            void _VLine(Pixmap<T>   &pxm,
                        unit_t       x0,
                        unit_t       y0,
                        unit_t       y1,
                        PUTPIXEL    &putPixel)
            {
                unit_t x1 = x0;
                if(y1<y0) cswap(y1,y0);
                if( Clip::Accept(x0, y0, x1, y1, *pxm) )
                {
                    assert(x0<=x1);
                    __VLine(pxm,x0,y0,y1-y0+1,putPixel);
                }
            }

            //! apply a solid color on a vertical line
            template <typename T>
            void VLine(Pixmap<T>   &pxm,
                       unit_t       x0,
                       unit_t       y0,
                       unit_t       y1,
                       typename Pixmap<T>::param_type C)
            {
                PutPixel::Copy<T> cb(C);
                _VLine(pxm, x0, y0, y1,cb);
            }

            //! apply a transparent color on a vertical line
            template <typename T>
            void VLine(Pixmap<T>   &pxm,
                       unit_t       x0,
                       unit_t       y0,
                       unit_t       y1,
                       typename Pixmap<T>::param_type C,
                       const uint8_t                  alpha)
            {
                PutPixel::Blend<T> cb(C,alpha);
                _VLine(pxm, x0, y0, y1,cb);
            }

        }


        namespace Draw {


            //! draw a line on an image
            template <typename T, typename PUTPIXEL>
            inline void __Line(Pixmap<T>    &img,
                               unit_t        x0,
                               unit_t        y0,
                               unit_t        x1,
                               unit_t        y1,
                               PUTPIXEL     &putPixel)
            {
                unit_t dx =  abs_of(x1-x0), sx = (x0<x1) ? 1 : -1;
                unit_t dy = -abs_of(y1-y0), sy = (y0<y1) ? 1 : -1;
                unit_t err = dx+dy, e2;                                   /* error value e_xy */
                assert(img->contains(x0,y0));
                assert(img->contains(x1,y1));
                for (;;){
                    /* loop */
                    putPixel(img[y0][x0]);
                    e2 = 2*err;
                    if (e2 >= dy) {                                         /* e_xy+e_x > 0 */
                        if (x0 == x1) break;
                        err += dy; x0 += sx;
                    }
                    if (e2 <= dx) {                                         /* e_xy+e_y < 0 */
                        if (y0 == y1) break;
                        err += dx; y0 += sy;
                    }
                }
            }

            //! clip and draw the line
            template <typename T, typename PROC>
            inline void _Line(Pixmap<T>    &img,
                              unit_t        x0,
                              unit_t        y0,
                              unit_t        x1,
                              unit_t        y1,
                              PROC         &proc)
            {
                if( Clip::Accept(x0,y0,x1,y1,*img) )
                {
                    __Line(img,x0,y0,x1,y1,proc);
                }
            }

            //! a line with a given color
            template <typename T>
            inline void Line(Pixmap<T>    &img,
                             unit_t        x0,
                             unit_t        y0,
                             unit_t        x1,
                             unit_t        y1,
                             typename Pixmap<T>::param_type color)
            {
                PutPixel::Copy<T> proc(color);
                _Line(img,x0,y0,x1,y1,proc);
            }


            //! a line with a given color/alpha
            template <typename T>
            inline void Line(Pixmap<T>    &img,
                             unit_t        x0,
                             unit_t        y0,
                             unit_t        x1,
                             unit_t        y1,
                             typename Pixmap<T>::param_type color,
                             const uint8_t                  alpha)
            {
                PutPixel::Blend<T> proc(color,alpha);
                _Line(img,x0,y0,x1,y1,proc);
            }


        }

    }
}



#endif

