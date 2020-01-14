//! \file
#ifndef Y_GRAPHIC_DRAW_CIRCLE_INCLUDED
#define Y_GRAPHIC_DRAW_CIRCLE_INCLUDED 1

#include "y/graphic/draw/putpixel.hpp"

namespace upsylon {

    namespace Graphic {

        namespace Draw {

            //! draw a circle on an image
            template <typename T,typename PROC>
            inline void _Circle(Pixmap<T>    &img,
                                const unit_t  xm,
                                const unit_t  ym,
                                unit_t        r,
                                PROC         &proc)
            {

                if(r<=0)
                {
                    assert(0==r);
                    if(img->contains(xm,ym))
                    {
                        const Point p(xm,ym);
                        proc(img[p],p);
                    }
                }
                else
                {
                    unit_t x = -r, y = 0, err = 2-2*r; /* bottom left to top right */
                    do {
                        const Point v[4] =
                        {
                            Point(xm-x,ym+y),  /*   I. Quadrant +x +y */
                            Point(xm-y,ym-x),  /*  II. Quadrant -x +y */
                            Point(xm+x,ym-y),  /* III. Quadrant -x -y */
                            Point(xm+y,ym+x)   /*  IV. Quadrant +x -y */
                        };
                        for(size_t i=0;i<4;++i)
                        {
                            const Point p = v[i];
                            if(img->contains(p))
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

            //! draw a colored circle on an image
            template <typename T>
            inline void Circle(Pixmap<T>    &img,
                               const unit_t  xm,
                               const unit_t  ym,
                               unit_t        r,
                               typename Pixmap<T>::param_type color)
            {
                PutPixel::Copy<T> proc(color);
                _Circle(img,xm,ym,r,proc);
            }


            //! draw a colored circle on an image
            template <typename T>
            inline void Circle(Pixmap<T>    &img,
                               const unit_t  xm,
                               const unit_t  ym,
                               unit_t        r,
                               typename Pixmap<T>::param_type color,
                               const uint8_t                  alpha)
            {
                PutPixel::Blend<T> proc(color,alpha);
                _Circle(img,xm,ym,r,proc);
            }

            //! gather mask
            template <typename T>
            inline void Circle(Pixmap<T>    &img,
                               const unit_t  xm,
                               const unit_t  ym,
                               unit_t        r,
                               Mask         &mask)
            {
                PutPixel::Store proc(mask);
                _Circle(img,xm,ym,r,proc);
            }

            
        }


        namespace Draw {

            //! clipping horizontal segement
            template <typename T, typename PROC>
            inline void __HSeg(Pixmap<T>    &img,
                               const unit_t  y,
                               const unit_t  xlo,
                               const unit_t  xhi,
                               PROC         &proc) throw()
            {
                assert(xlo<=xhi);
                Point p(xlo,y);
                for(;p.x<=xhi;++p.x)
                {
                    if(img->contains(p))
                    {
                        proc(img[p],p);
                    }
                }
            }

            //! generic disk
            template <typename T, typename PROC>
            inline void _Disk(Pixmap<T>    &img,
                              const unit_t  xm,
                              const unit_t  ym,
                              unit_t        r,
                              PROC         &proc) throw()
            {
                if(r<=0)
                {
                    assert(0==r);
                    if(img->contains(xm,ym))
                    {
                        const Point p(xm,ym);
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

            //! draw a colored disk on an image
            template <typename T>
            inline void Disk(Pixmap<T>    &img,
                             const unit_t  xm,
                             const unit_t  ym,
                             unit_t        r,
                             typename Pixmap<T>::param_type color)
            {
                PutPixel::Copy<T> proc(color);
                _Disk(img,xm,ym,r,proc);
            }


            //! draw a colored/alpha disk on an image
            template <typename T>
            inline void Disk(Pixmap<T>    &img,
                             const unit_t  xm,
                             const unit_t  ym,
                             unit_t        r,
                             typename Pixmap<T>::param_type color,
                             const uint8_t alpha)
            {
                PutPixel::Blend<T> proc(color,alpha);
                _Disk(img,xm,ym,r,proc);
            }

            //! gather mask
            template <typename T>
            inline void Disk(Pixmap<T>    &img,
                             const unit_t  xm,
                             const unit_t  ym,
                             unit_t        r,
                             Mask         &mask)
            {
                PutPixel::Store proc(mask);
                _Disk(img,xm,ym,r,proc);
            }

        }

    }

}

#endif

