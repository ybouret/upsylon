//! \file
#ifndef Y_INK_OPS_FILTER_INCLUDED
#define Y_INK_OPS_FILTER_INCLUDED 1

#include "y/ink/pixmap.hpp"
#include "y/ink/pixel.hpp"
#include "y/container/matrix.hpp"

namespace upsylon
{
    namespace ink
    {

        //! to apply function on pixmaps
        class filter
        {
        public:
            //! target = func(source), with an engine
            template <typename T,typename U,typename FUNC> static inline
            void apply(pixmap<T>       &target,
                       const pixmap<U> &source,
                       FUNC            &func,
                       engine          &E)
            {
                parameters<T,U,FUNC> proxy = { &target, &source, &func };
                E.run(proxy);
            }

            //! apply a matrix as a stencil
            template <typename T, typename U> static inline
            void stencil(pixmap<float>   &target,
                         const pixmap<T> &source,
                         const matrix<U> &block,
                         engine          &E)
            {
                block_parameters<T,U> proxy = { &target, &source, &block };
                E.run(proxy);
            }

            template <typename T> static inline
            T find_min( const pixmap<T> &source, engine &E )
            {
                E.acquire_all(sizeof(T));
                {
                    __find_min<T> proxy = { &source };
                    E.run(proxy);
                }
                const array<tile> &zones = E.zones;
                size_t n   = zones.size();
                T      ans = zones[n].cache.get<T>();
                for(--n;n>0;--n)
                {
                    const T tmp = zones[n].cache.get<T>();
                    if(tmp<ans) ans=tmp;
                }
                return ans;
            }

            template <typename T> static inline
            T find_max( const pixmap<T> &source, engine &E )
            {
                E.acquire_all(sizeof(T));
                {
                    __find_max<T> proxy = { &source };
                    E.run(proxy);
                }
                const array<tile> &zones = E.zones;
                size_t n   = zones.size();
                T      ans = zones[n].cache.get<T>();
                for(--n;n>0;--n)
                {
                    const T tmp = zones[n].cache.get<T>();
                    if(tmp>ans) ans=tmp;
                }
                return ans;
            }

            template <typename T> static inline
            void find_min_max( T &vmin, T&vmax, const pixmap<T> &source, engine &E )
            {
                E.acquire_all(2*sizeof(T));
                {
                    __find_min_max<T> proxy = { &source };
                    E.run(proxy);
                }
                const array<tile> &zones = E.zones;
                size_t n   = zones.size();
                vmin = zones[n].cache.get<T>(0);
                vmax = zones[n].cache.get<T>(1);
                for(--n;n>0;--n)
                {
                    {
                        const T tmp = zones[n].cache.get<T>(0);
                        if(tmp<vmin)
                        {
                            vmin = tmp;
                        }
                    }
                    {
                        const T tmp = zones[n].cache.get<T>(1);
                        if(vmax<tmp)
                        {
                            vmax = tmp;
                        }
                    }
                }
            }

            
            template <typename T, typename F2T> static inline
            void rescale( pixmap<T> &source, const T vmin, const T vmax, F2T &float2type, engine &E)
            {
                __rescale<T,F2T> proxy = { &source, vmin, vmax, &float2type };
                E.run(proxy);
            }



        private:
            template <typename T,typename U,typename FUNC>
            struct parameters
            {
                pixmap<T>       *_target;
                const pixmap<U> *_source;
                FUNC            *_func;

                inline void operator()(const area &zone, lockable &)
                {
                    assert(_target);
                    assert(_source);
                    assert(_func);
                    pixmap<T>       &target = * _target;
                    const pixmap<U> &source = * _source;
                    FUNC            &func   = * _func;
                    Y_INK_AREA_LIMITS(zone);

                    for(unit_t y=ymax;y>=ymin;--y)
                    {
                        typename       pixmap<T>::row &tgt = target[y];
                        const typename pixmap<U>::row &src = source[y];
                        for(unit_t x=xmax;x>=xmin;--x)
                        {
                            tgt[x] = func(src[x]);
                        }
                    }
                }
            };

            template <typename T, typename U>
            struct block_parameters
            {
                pixmap<float>   *_target;
                const pixmap<T> *_source;
                const matrix<U> *_block;

                inline void operator()(const tile &zone, lockable &)
                {
                    assert(_target);assert(_source);assert(_block);
                    if(zone.pixels)
                    {
                        pixmap<float>   &target = * _target;
                        const pixmap<T> &source = * _source;
                        const matrix<U> &block  = * _block;

                        const unit_t mx = unit_t(block.cols);
                        const unit_t my = unit_t(block.rows);
                        const unit_t dx = (mx-1)/2;
                        const unit_t dy = (my-1)/2;
                        Y_INK_AREA_LIMITS(zone);
                        for(unit_t y=ymax;y>=ymin;--y)
                        {
                            pixmap<float>::row            &tgt = target[y];
                            const unit_t local_y=y-dy;
                            for(unit_t x=xmax;x>=xmin;--x)
                            {
                                float ans = 0;
                                const unit_t local_x=x-dx;
                                for(unit_t iy=1,ly=local_y;iy<=my;++iy,++ly)
                                {
                                    const unit_t                   zfy   = source.zfy(ly);
                                    const typename pixmap<T>::row &src_y = source[ zfy ];
                                    const array<U>                &blk_y = block[iy];
                                    for(unit_t ix=1,lx=local_x;ix<=mx;++ix,++lx)
                                    {
                                        const unit_t zfx = source.zfx(lx);
                                        const float  b   = float(blk_y[ix]);
                                        const float  s   = float(src_y[zfx]);
                                        ans += b*s;
                                    }
                                }
                                tgt[x] = ans;
                            }
                        }
                    }
                }
            };

            template <typename T>
            struct __find_min
            {
                const pixmap<T> *_source;

                inline void operator()( const tile &zone, lockable &)
                {
                    assert(_source);
                    if(zone.pixels)
                    {
                        const pixmap<T> &source = * _source;
                        Y_INK_AREA_LIMITS(zone);


                        T vmin = source[ymin][xmin];
                        for(unit_t y=ymax;y>=ymin;--y)
                        {
                            const typename pixmap<T>::row &tgt = source[y];
                            for(unit_t x=xmax;x>=xmin;--x)
                            {
                                const T v = tgt[x];
                                if(v<vmin) vmin = v;
                            }
                        }
                        zone.cache.get<float>(0) = vmin;
                    }
                }
            };

            template <typename T>
            struct __find_max
            {
                const pixmap<T> *_source;

                inline void operator()( const tile &zone, lockable &)
                {
                    assert(_source);
                    if(zone.pixels)
                    {
                        const pixmap<T> &source = * _source;
                        Y_INK_AREA_LIMITS(zone);
                        T vmax = source[ymin][xmin];
                        for(unit_t y=ymax;y>=ymin;--y)
                        {
                            const typename pixmap<T>::row &tgt = source[y];
                            for(unit_t x=xmax;x>=xmin;--x)
                            {
                                const T v = tgt[x];
                                if(v>vmax) vmax= v;
                            }
                        }
                        zone.cache.get<float>(0) = vmax;
                    }
                }
            };

            template <typename T>
            struct __find_min_max
            {
                const pixmap<T> *_source;

                inline void operator()( const tile &zone, lockable &)
                {
                    assert(_source);
                    if(zone.pixels)
                    {
                        const pixmap<T> &source = * _source;
                        Y_INK_AREA_LIMITS(zone);
                        T vmax = source[ymin][xmin], vmin=vmax;
                        for(unit_t y=ymax;y>=ymin;--y)
                        {
                            const typename pixmap<T>::row &tgt = source[y];
                            for(unit_t x=xmax;x>=xmin;--x)
                            {
                                const T v = tgt[x];
                                if(vmax<v)      vmax=v;
                                else if(v<vmin) vmin=v;
                            }
                        }
                        zone.cache.get<float>(0) = vmin;
                        zone.cache.get<float>(1) = vmax;
                    }
                }
            };

            template <typename T,typename F2T>
            struct __rescale
            {
                pixmap<T> *_source;
                T          _vmin;
                T          _vmax;
                F2T       *_func;

                inline void operator()(const tile &zone, lockable &)
                {
                    assert(_source);
                    assert(_func);
                    if(zone.pixels)
                    {
                        pixmap<T> &source = *_source;
                        F2T       &func   = *_func;

                        const float vmin   = float(_vmin);
                        const float vmax   = float(_vmax);
                        Y_INK_AREA_LIMITS(zone);

                        if(vmax>vmin)
                        {
                            const float scale = 1.0f/(vmax-vmin);
                            for(unit_t y=ymax;y>=ymin;--y)
                            {
                                typename pixmap<T>::row &tgt = source[y];
                                for(unit_t x=xmax;x>=xmin;--x)
                                {
                                    tgt[x] = func(scale*(float(tgt[x])-vmin));
                                }
                            }
                        }
                        else
                        {
                            for(unit_t y=ymax;y>=ymin;--y)
                            {
                                typename pixmap<T>::row &tgt = source[y];
                                for(unit_t x=xmax;x>=xmin;--x)
                                {
                                    tgt[x] = 0;
                                }
                            }
                        }
                    }
                }
            };


        };

    };
}


#endif

