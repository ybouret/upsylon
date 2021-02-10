
#ifndef Y_GFX_ASYNC_MINMAX_INCLUDED
#define Y_GFX_ASYNC_MINMAX_INCLUDED 1

#include "y/gfx/async/broker.hpp"


namespace upsylon
{
    namespace GFX
    {
        template <typename T> class Pixmap;
        
        namespace Async
        {
            template <typename T>
            class _MinMax
            {
            public:
                inline  _MinMax() throw() : vmax(0), pmax(), vmin(0), pmin() {}
                inline ~_MinMax() throw() {}
                T      vmax;
                Point  pmax;
                T      vmin;
                Point  pmin;
                
                static inline Point FindMin( T &value, const accessible<parallel> &arr)
                {
                    const _MinMax *info = & arr[1]._<_MinMax>();
                    Point          p = info->pmin;
                    T              v = info->vmin;
                    for(size_t i=arr.size();i>1;--i)
                    {
                        info = & arr[i]._<_MinMax>();
                        const T t = info->vmin;
                        if(t<v)
                        {
                            v = t;
                            p = info->pmin;
                        }
                    }
                    value = v;
                    return p;
                }
                
                static inline Point FindMax( T &value, const accessible<parallel> &arr)
                {
                    const _MinMax *info = & arr[1]._<_MinMax>();
                    Point          p = info->pmax;
                    T              v = info->vmax;
                    for(size_t i=arr.size();i>1;--i)
                    {
                        info = & arr[i]._<_MinMax>();
                        const T t = info->vmax;
                        if(t>v)
                        {
                            v = t;
                            p = info->pmax;
                        }
                    }
                    value = v;
                    return p;
                }
                
                
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(_MinMax);
                
            };
            
            template <typename T,typename U, typename FUNC>
            struct MinMax
            {
                const Pixmap<U> &source;
                FUNC            &func;
                
                
                static inline void runMax(Async::Worker &worker,
                                          lockable      &,
                                          void          *data)
                {
                    assert(data);
                    MinMax          &self   = *static_cast<MinMax *>(data);
                    const Pixmap<U> &source = self.source;
                    const Tile      &tile   = worker.tile;
                    FUNC            &func   = self.func;
                    Point            pmax   = tile[1].begin;
                    T                vmax   = func( source[pmax] );
                    
                    for(size_t t=tile.size();t>0;--t)
                    {
                        const HScan                   &s   = tile[t];
                        Point                          p   = s.begin;
                        const typename Pixmap<U>::Row &src = source[p.y];
                        for(unit_t i=s.width;i>0;--i,++p.x)
                        {
                            const T temp = func(src[p.x]);
                            if(temp>vmax)
                            {
                                vmax = temp;
                                pmax = p;
                            }
                        }
                    }
                    
                    typedef _MinMax<T> store_t;
                    store_t &result = worker._<store_t>();
                    result.vmax     = vmax;
                    result.pmax     = pmax;
                }
                
                static inline void runMin(Async::Worker &worker,
                                          lockable      &,
                                          void          *data)
                {
                    assert(data);
                    MinMax          &self   = *static_cast<MinMax *>(data);
                    const Pixmap<U> &source = self.source;
                    const Tile      &tile   = worker.tile;
                    FUNC            &func   = self.func;
                    Point            pmin   = tile[1].begin;
                    T                vmin   = func( source[pmin] );
                    
                    for(size_t t=tile.size();t>0;--t)
                    {
                        const HScan                   &s   = tile[t];
                        Point                          p   = s.begin;
                        const typename Pixmap<U>::Row &src = source[p.y];
                        for(unit_t i=s.width;i>0;--i,++p.x)
                        {
                            const T temp = func(src[p.x]);
                            if(temp<vmin)
                            {
                                vmin = temp;
                                pmin = p;
                            }
                        }
                    }
                    
                    typedef _MinMax<T> store_t;
                    store_t &result = worker._<store_t>();
                    result.vmin     = vmin;
                    result.pmin     = pmin;
                }
                
            };
            
            template <typename T, typename U, typename FUNC>
            inline Point FindMax( T &value, const Pixmap<U> &pixmap, FUNC &func, Async::Broker &broker)
            {
                typedef _MinMax<T> store_t;
                parallel::group   &cache = *(broker.engine);
                cache.make<store_t>();
                MinMax<T,U,FUNC> op = { pixmap, func };
                value = 0;
                broker(op.runMax,&op);
                return store_t::FindMax(value,cache);
            }
            
            template <typename T, typename U, typename FUNC>
            inline Point FindMin( T &value, const Pixmap<U> &pixmap, FUNC &func, Async::Broker &broker)
            {
                typedef _MinMax<T> store_t;
                parallel::group   &cache = *(broker.engine);
                cache.make<store_t>();
                MinMax<T,U,FUNC> op = { pixmap, func };
                value = 0;
                broker(op.runMin,&op);
                return store_t::FindMin(value,cache);
            }
            
        }
        
    }
}
#endif

