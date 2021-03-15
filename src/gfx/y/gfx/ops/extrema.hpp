
//! \file

#ifndef Y_GFX_OPS_EXTREMA_INCLUDED
#define Y_GFX_OPS_EXTREMA_INCLUDED 1

#include "y/gfx/pixmap.hpp"
 
namespace upsylon
{
    namespace graphic
    {

        struct extrema
        {
            template <typename T>
            static inline
            T get_max(const pixmap<T> &pxm, broker &apply )
            {
                apply.caches.make<T>();
                apply(kernel_maxi<T>,(void*)&pxm);
                return apply.caches.get_max<T>();
            }

            template <typename T>
            static inline
            T get_min(const pixmap<T> &pxm, broker &apply )
            {
                apply.caches.make<T>();
                apply(kernel_mini<T>,(void*)&pxm);
                return apply.caches.get_min<T>();
            }

        private:
            template <typename T> static inline
            void kernel_maxi(const tile &t, void *args, lockable &) throw()
            {
                assert(args);
                const pixmap<T> &pxm = *static_cast< const pixmap<T> *>(args);
                T                res = pxm( t.origin() );
                for(size_t j=t.size();j>0;--j)
                {
                    const segment   &s    = t[j];
                    const pixrow<T> &r    = pxm(s.y);
                    const unit_t     xmin = s.xmin;

                    for(unit_t x=s.xmax;x>=xmin;--x)
                    {
                        const T tmp = r(x);
                        if(tmp>res) res = tmp;
                    }
                }
                t.cache->as<T>() = res;
            }

            template <typename T> static inline
            void kernel_mini(const tile &t, void *args, lockable &) throw()
            {
                assert(args);
                const pixmap<T> &pxm = *static_cast< const pixmap<T> *>(args);
                T                res = pxm( t.origin() );
                for(size_t j=t.size();j>0;--j)
                {
                    const segment   &s    = t[j];
                    const pixrow<T> &r    = pxm(s.y);
                    const unit_t     xmin = s.xmin;

                    for(unit_t x=s.xmax;x>=xmin;--x)
                    {
                        const T tmp = r(x);
                        if(tmp<res) res = tmp;
                    }
                }
                t.cache->as<T>() = res;
            }



        };


    }

}

#endif

