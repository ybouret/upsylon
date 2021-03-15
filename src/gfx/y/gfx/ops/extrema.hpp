
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
                assert( pxm.has_same_metrics_than(apply) );

                apply.caches.make<T>();
                apply(kernel_maxi<T>,(void*)&pxm);
                return apply.caches.get_max<T>();
            }

            template <typename T>
            static inline
            T get_min(const pixmap<T> &pxm, broker &apply )
            {
                assert( pxm.has_same_metrics_than(apply) );

                apply.caches.make<T>();
                apply(kernel_mini<T>,(void*)&pxm);
                return apply.caches.get_min<T>();
            }

            template <typename T>
            static inline
            const T * minmax(const pixmap<T> &pxm, broker &apply)
            {
                assert( pxm.has_same_metrics_than(apply) );

                apply.caches.make<T>(2);
                apply(kernel_both<T>,(void*)&pxm);
                return apply.caches.minmax<T>();
            }

            template <typename T>
            static inline
            void normalize(pixmap<T>       &target,
                           const pixmap<T> &source,
                           broker          &apply,
                           const T          vmin,
                           const T          vmax) throw()
            {
                assert( target.has_same_metrics_than(source) );
                assert( target.has_same_metrics_than(apply)  );

                struct ops
                {
                    pixmap<T>       &target;
                    const pixmap<T> &source;
                    T                vmin;
                    T                vmax;


                    static inline void run(const tile &t,
                                           void       *args,
                                           lockable   &) throw()
                    {
                        assert(args);
                        static const T   _0(0);
                        static const T   _1(1);
                        ops             &self   = *static_cast<ops *>(args);
                        pixmap<T>       &target = self.target;
                        const pixmap<T> &source = self.source;
                        const T          vmin   = self.vmin;
                        const T          vmax   = self.vmax;
                        const T          vamp   = vmax-vmin;

                        for(size_t j=t.size();j>0;--j)
                        {
                            const segment   &s = t[j];
                            const unit_t     xmin  = s.xmin;
                            const unit_t     y     = s.y;
                            pixrow<T>       &tgt   = target(y);
                            const pixrow<T> &src   = source(y);
                            for(unit_t x=s.xmax;x>=xmin;--x)
                            {
                                const T v = src(x);
                                if(v<=vmin)
                                {
                                    tgt(x) = _0;
                                }
                                else
                                {
                                    if(vmax<=v)
                                    {
                                        tgt(x) = _1;
                                    }
                                    else
                                    {
                                        tgt(x) = clamp(_0,(v-vmin)/vamp,_1);
                                    }
                                }

                            }
                        }
                    }

                };

                ops todo = { target, source, vmin, vmax };
                apply( ops::run, &todo );
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

            template <typename T> static inline
            void kernel_both(const tile &t, void *args, lockable &) throw()
            {
                assert(args);
                const pixmap<T> &pxm = *static_cast< const pixmap<T> *>(args);
                T                vmin = pxm( t.origin() );
                T                vmax = vmin;
                for(size_t j=t.size();j>0;--j)
                {
                    const segment   &s    = t[j];
                    const pixrow<T> &r    = pxm(s.y);
                    const unit_t     xmin = s.xmin;

                    for(unit_t x=s.xmax;x>=xmin;--x)
                    {
                        const T tmp = r(x);
                        if(tmp<vmin) vmin = tmp;
                        if(tmp>vmax) vmax = tmp;
                    }
                }
                assert(t.cache->is<T>());
                assert(t.cache->tell()>=2);
                t.cache->get<T>(1) = vmin;
                t.cache->get<T>(2) = vmax;
            }

        };


    }

}

#endif

