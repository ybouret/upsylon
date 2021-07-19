

//! \file

#ifndef Y_GFX_OPS_INTENSITY_INCLUDED
#define Y_GFX_OPS_INTENSITY_INCLUDED 1

#include "y/gfx/pixmap.hpp"
#include "y/gfx/color/convert.hpp"
#include "y/gfx/pixel.hpp"

namespace upsylon
{
    namespace graphic
    {

        //______________________________________________________________________
        //
        //! handle intensites
        //______________________________________________________________________
        class intensity
        {
        public:
            intensity() throw();
            virtual ~intensity() throw();

            const float vmin;
            const float vmax;
            const float scal;

            template <typename T> inline
            void scan(const pixmap<T> &source, broker &apply)
            {
                apply.caches.make<float>(4);
                apply(kernel_scan<T>,(void*)&source);
                load(apply);
            }

            void format(broker &) const;

            template <typename T> inline
            void enhance(pixmap<T> &target, const pixmap<T> &source, broker &apply) const
            {
                if(scal>0.0f)
                {
                    struct ops
                    {
                        pixmap<T>       &target;
                        const pixmap<T> &source;
                        const intensity &data;

                        static inline
                        void run(const tile &t, void *args, lockable &) throw()
                        {
                            ops             &self   = *static_cast<ops*>(args);
                            pixmap<T>       &target = self.target;
                            const pixmap<T> &source = self.source;
                            const intensity &data   = self.data;
                            const float      vmin   = data.vmin;
                            const float      vmax   = data.vmax;
                            const float      scal   = data.scal;

                            for(size_t j=t.size();j>0;--j)
                            {
                                const segment   &s     = t[j];
                                const unit_t     y     = s.y;
                                const pixrow<T> &src_y = source(y);
                                pixrow<T>       &tgt_y = target(y);
                                const unit_t     xmin = s.xmin;

                                for(unit_t x=s.xmax;x>=xmin;--x)
                                {
                                    const T    &src  = src_y(x);
                                    T          &tgt  = tgt_y(x);
                                    const float vcur = convert<float,T>::from( src ); // current intensity
                                    if(vcur<=vmin)
                                    {
                                        tgt = pixel::zero<T>();
                                    }
                                    else
                                    {
                                        if(vcur>=vmax)
                                        {
                                            tgt = pixel::opaque<T>();
                                        }
                                        else
                                        {
                                            (void)scal;
                                            tgt = src;
                                        }
                                    }
                                }
                            }
                        }

                    };

                    ops todo = { target, source, *this };
                    apply(ops::run,&todo);

                }
                else
                {
                    target.ldz();
                }
            }


        private:
            template <typename T> static inline
            void kernel_scan(const tile &t, void *args, lockable &) throw()
            {
                const pixmap<T> &pxm  = *static_cast< const pixmap<T> *>(args);
                float            vmin = convert<float,T>::from(pxm( t.origin() ));
                float            vmax = vmin;
                for(size_t j=t.size();j>0;--j)
                {
                    const segment   &s    = t[j];
                    const pixrow<T> &r    = pxm(s.y);
                    const unit_t     xmin = s.xmin;

                    for(unit_t x=s.xmax;x>=xmin;--x)
                    {
                        const float tmp = convert<float,T>::from( r(x) );
                        if(tmp<vmin) vmin = tmp;
                        if(tmp>vmax) vmax = tmp;
                    }
                }
                assert(t.cache->is<float>());
                assert(t.cache->tell()>=2);
                t.cache->get<float>(1) = vmin;
                t.cache->get<float>(2) = vmax;
            }

            void load(const broker &apply) throw();

            template <typename T> static inline
            void kernel_norm(const tile &t, void *args, lockable &) throw()
            {
                pixmap<T>       &pxm  = *static_cast<pixmap<T> *>(args);
                const float      vmin = t.cache->get<float>(1);
                const float      vmax = t.cache->get<float>(2);
                const float      scal = t.cache->get<float>(3);

                for(size_t j=t.size();j>0;--j)
                {
                    const segment   &s    = t[j];
                    pixrow<T>       &r    = pxm(s.y);
                    const unit_t     xmin = s.xmin;

                    for(unit_t x=s.xmax;x>=xmin;--x)
                    {
                        T          &src  = r(x);
                        const float vcur = convert<float,T>::from( src ); // current intensity
                        if(vcur<=vmin)
                        {
                            src = pixel::zero<T>();
                        }
                        else
                        {
                            if(vcur>=vmax)
                            {
                                src = pixel::opaque<T>();
                            }
                            else
                            {
                                (void)scal;
                            }
                        }
                    }
                }
            }

        };

    }

}

#endif

