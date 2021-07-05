//! \file
#ifndef Y_GRAPHIC_DRAW_CIRCLE_INCLUDED
#define Y_GRAPHIC_DRAW_CIRCLE_INCLUDED 1

#include "y/gfx/draw/putpixel.hpp"
#include "y/gfx/pixmap.hpp"

namespace upsylon {

    namespace graphic {

        namespace draw {

            //! draw a circle on an image
            template <typename T,typename PROC>
            inline void _circle(pixmap<T>    &img,
                                const unit_t  xm,
                                const unit_t  ym,
                                unit_t        r,
                                PROC         &proc)
            {

                if(r<=0)
                {
                    assert(0==r);
                    if(img.owns(xm,ym))
                    {
                        const coord p(xm,ym);
                        proc(img[p],p);
                    }
                }
                else
                {
                    unit_t x = -r, y = 0, err = 2-2*r; /* bottom left to top right */
                    do {
                        const coord v[4] =
                        {
                            coord(xm-x,ym+y),  /*   I. Quadrant +x +y */
                            coord(xm-y,ym-x),  /*  II. Quadrant -x +y */
                            coord(xm+x,ym-y),  /* III. Quadrant -x -y */
                            coord(xm+y,ym+x)   /*  IV. Quadrant +x -y */
                        };
                        for(size_t i=0;i<4;++i)
                        {
                            const coord p = v[i];
                            if(img.owns(p))
                            {
                                proc(img[p],p);
                            }
                        }
                        r = err;
                        if (r <= y)
                        {
                            err += ++y*2+1;            /* e_xy+e_y < 0 */
                        }
                        if (r > x || err > y)
                        {/* e_xy+e_x > 0 or no 2nd y-step */
                            err += ++x*2+1;
                        }/* -> x-step now */
                    } while (x < 0);
                }
            }
            
#define Y_CIRCLE_ARGS pixmap<T> &pxm, const unit_t  xm, const unit_t ym, const unit_t r //!< args for circle
#define Y_CIRCLE_CALL _circle(pxm,xm,ym,r,proc)                                         //!< call for circle

            Y_GFX_DRAW_IMPL(circle,Y_CIRCLE_ARGS,Y_CIRCLE_CALL)

#undef Y_CIRCLE_CALL
#undef Y_CIRCLE_ARGS


            
        }


        namespace draw {

            //! clipping horizontal segement
            template <typename T, typename PROC>
            inline void __HSeg(pixmap<T>    &img,
                               const unit_t  y,
                               const unit_t  xlo,
                               const unit_t  xhi,
                               PROC         &proc) throw()
            {
                assert(xlo<=xhi);
                coord p(xlo,y);
                for(;p.x<=xhi;++p.x)
                {
                    if(img.owns(p))
                    {
                        proc(img[p],p);
                    }
                }
            }

            //! generic disk
            template <typename T, typename PROC>
            inline void _disk(pixmap<T>    &img,
                              const unit_t  xm,
                              const unit_t  ym,
                              unit_t        r,
                              PROC         &proc) throw()
            {
                if(r<=0)
                {
                    assert(0==r);
                    if(img.owns(xm,ym))
                    {
                        const coord p(xm,ym);
                        proc(img[p],p);
                    }
                }
                else
                {
                    unit_t x = -r, y = 0, err = 2-2*r; /* bottom left to top right */
                    bool new_y = true;
                    do {
                        if(new_y)
                        {
                            __HSeg(img,ym-y,xm+x,xm-x,proc);
                            if(y>0)
                            {
                                __HSeg(img,ym+y,xm+x,xm-x,proc);
                            }
                            new_y = false;
                        }
                        r = err;
                        if (r <= y)
                        {
                            err += ++y*2+1;            /* e_xy+e_y < 0 */
                            new_y = true;
                        }
                        if (r > x || err > y)
                        {/* e_xy+e_x > 0 or no 2nd y-step */
                            err += ++x*2+1;                    /* -> x-step now */
                        }
                    } while (x < 0);
                }
            }


#define Y_DISK_ARGS pixmap<T> &pxm, const unit_t  xm, const unit_t ym, const unit_t r //!< args for disk
#define Y_DISK_CALL _disk(pxm,xm,ym,r,proc)                                           //!< call for disk

            Y_GFX_DRAW_IMPL(disk,Y_DISK_ARGS,Y_DISK_CALL)

#undef Y_DISK_CALL
#undef Y_DISK_ARGS


        }

    }

}

#endif

