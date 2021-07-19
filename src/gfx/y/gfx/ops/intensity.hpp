

//! \file

#ifndef Y_GFX_OPS_INTENSITY_INCLUDED
#define Y_GFX_OPS_INTENSITY_INCLUDED 1

#include "y/gfx/pixmap.hpp"
#include "y/gfx/color/convert.hpp"

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

            template <typename T> inline
            void scan(const pixmap<T> &source, broker &apply)
            {
                apply.caches.make<float>(2);
                apply(kernel_scan<T>,(void*)&source);
                load(apply);
            }

            void format(broker &) const;

            template <typename T> inline
            void enhance(pixmap<T> &target, broker &apply) const
            {
                if(vmax>vmin)
                {
                    format(apply);
                    apply(kernel_norm<T>,(void*)&target);
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
                const pixmap<T> &pxm  = *static_cast< const pixmap<T> *>(args);
                const float      vmin = t.cache->get<float>(1);
                const float      vmax = t.cache->get<float>(2);

                for(size_t j=t.size();j>0;--j)
                {
                    const segment   &s    = t[j];
                    const pixrow<T> &r    = pxm(s.y);
                    const unit_t     xmin = s.xmin;

                    for(unit_t x=s.xmax;x>=xmin;--x)
                    {
                        T          &src  = r(x);
                        const float vtmp = convert<float,T>::from( src );
                        if(vtmp<=vmin)
                        {

                        }
                        else
                        {
                            if(vtmp>=vmax)
                            {
                                
                            }
                            else
                            {

                            }
                        }
                    }
                }
            }

        };

    }

}

#endif

