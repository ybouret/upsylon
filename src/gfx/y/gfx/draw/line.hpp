
//! \file

#ifndef Y_GRAPHIC_DRAW_LINE
#define Y_GRAPHIC_DRAW_LINE 1

#include "y/gfx/draw/clip.hpp"
#include "y/gfx/draw/putpixel.hpp"
#include "y/gfx/pixmap.hpp"

namespace  upsylon {

    namespace graphic {

        namespace draw {

            //! draw an unchecked Horizontal Line
            /**
             putPixel(T,coord)
             */
            template <typename T, typename PUTPIXEL> inline
            void __hline(pixmap<T>   &pxm,
                         const unit_t x0,
                         const unit_t y0,
                         const size_t w,
                         PUTPIXEL    &putPixel)
            {
                typename pixmap<T>::row &row = pxm[y0];
                for(size_t i=0;i<w;++i)
                {
                    const coord p(x0+i,y0);
                    putPixel(row[p.x],p);
                }
            }

            //! apply putpixel on a clipped horizontal line
            template <typename T, typename PUTPIXEL> inline
            void _hline(pixmap<T>   &pxm,
                        unit_t       x0,
                        unit_t       y0,
                        unit_t       x1,
                        PUTPIXEL    &putPixel)
            {
                unit_t          y1=y0;
                if(x1<x0) cswap(x1,x0);
                if( clip::accept(x0,y0,x1,y1,pxm) )
                {
                    assert(x0<=x1);
                    __hline(pxm,x0,y0,x1-x0+1,putPixel);
                }
            }


#define Y_GFX_DRAW_IMPL(NAME,ARGS,CALL) \
/* put color  */         \
/**/    template <typename T> \
/**/    void NAME(ARGS,\
/**/              typename type_traits<T>::parameter_type C)\
/**/    {\
/**/        const putpixel::copy<T> proc(C);\
/**/        CALL;\
/**/    } \
/* blend color  */         \
/**/    template <typename T> \
/**/    void NAME(ARGS,\
/**/              typename type_traits<T>::parameter_type C,\
/**/              const uint8_t alpha)\
/**/    {\
/**/        const putpixel::blend<T> proc(C,alpha);\
/**/        CALL;\
/**/    }\
/* feed mask  */              \
/**/    template <typename T> \
/**/    void NAME(ARGS,\
/**/             mask &m)\
/**/    {\
/**/        putpixel::store proc(m);\
/**/        CALL;\
/**/    }


#define Y_HLINE_ARGS pixmap<T> &pxm, const unit_t x0, const unit_t y0, const unit_t x1
#define Y_HLINE_CALL _hline(pxm,x0,y0,x1,proc)

            Y_GFX_DRAW_IMPL(hline,Y_HLINE_ARGS,Y_HLINE_CALL)

#undef Y_HLINE_CALL
#undef Y_HLINE_ARGS


        }

        namespace draw {

            //! unsafe vertical line
            template <typename T, typename PUTPIXEL> inline
            void __vline(pixmap<T>   &pxm,
                         const unit_t x0,
                         const unit_t y0,
                         const size_t h,
                         PUTPIXEL    &putPixel)
            {
                for(size_t j=0;j<h;++j)
                {
                    const coord p(x0,y0+j);
                    putPixel(pxm[p],p);
                }
            }

            //! apply putpixel on a clipped vertical line
            template <typename T, typename PUTPIXEL> inline
            void _vline(pixmap<T>   &pxm,
                        unit_t       x0,
                        unit_t       y0,
                        unit_t       y1,
                        PUTPIXEL    &putPixel)
            {
                unit_t          x1=x0;
                if(y1<y0) cswap(y1,y0);
                if( clip::accept(x0,y0,x1,y1,pxm) )
                {
                    assert(x0<=x1);
                    __vline(pxm,x0,y0,y1-y0+1,putPixel);
                }
            }


#define Y_VLINE_ARGS pixmap<T> &pxm, const unit_t x0, const unit_t y0, const unit_t y1
#define Y_VLINE_CALL _vline(pxm,x0,y0,y1,proc)

            Y_GFX_DRAW_IMPL(vline,Y_VLINE_ARGS,Y_VLINE_CALL)

#undef Y_VLINE_CALL
#undef Y_VLINE_ARGS

        }


        namespace draw {


            //! draw a line on an image
            template <typename T, typename PUTPIXEL>
            inline void __Line(pixmap<T>    &img,
                               unit_t        x0,
                               unit_t        y0,
                               unit_t        x1,
                               unit_t        y1,
                               PUTPIXEL     &putPixel)
            {
                unit_t dx  =  abs_of(x1-x0), sx = (x0<x1) ? 1 : -1;
                unit_t dy  = -abs_of(y1-y0), sy = (y0<y1) ? 1 : -1;
                unit_t err = dx+dy, e2;                                   /* error value e_xy */
                assert(img.owns(x0,y0));
                assert(img.owns(x1,y1));
                for (;;){
                    /* loop */
                    const coord p(x0,y0);
                    putPixel(img[p],p);
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
            inline void _Line(pixmap<T>    &img,
                              unit_t        x0,
                              unit_t        y0,
                              unit_t        x1,
                              unit_t        y1,
                              PROC         &proc)
            {
                if( clip::accept(x0,y0,x1,y1,img) )
                {
                    __Line(img,x0,y0,x1,y1,proc);
                }
            }

#define Y_LINE_ARGS pixmap<T> &pxm, const unit_t x0, const unit_t y0, const unit_t x1, const unit_t y1
#define Y_LINE_CALL _Line(pxm,x0,y0,x1,y1,proc)

            Y_GFX_DRAW_IMPL(line,Y_LINE_ARGS,Y_LINE_CALL)

#undef Y_LINE_CALL
#undef Y_LINE_ARGS



        }

    }
}



#endif

