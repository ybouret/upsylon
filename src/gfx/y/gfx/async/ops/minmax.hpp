
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
            
            struct MinMax
            {
                
                template <typename T>
                class Info
                {
                public:
                    inline  Info() throw() : vmax(0), pmax(), vmin(0), pmin() {}
                    inline ~Info() throw() {}
                    T      vmax;
                    Point  pmax;
                    T      vmin;
                    Point  pmin;
                    
                    static inline Point FindMin( T &value, const accessible<parallel> &arr)
                    {
                        const Info *info = & arr[1]._<Info>();
                        Point       p    = info->pmin;
                        T           v    = info->vmin;
                        for(size_t i=arr.size();i>1;--i)
                        {
                            info = & arr[i]._<Info>();
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
                        const Info *info = & arr[1]._<Info>();
                        Point       p = info->pmax;
                        T           v = info->vmax;
                        for(size_t i=arr.size();i>1;--i)
                        {
                            info = & arr[i]._<Info>();
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
                    Y_DISABLE_COPY_AND_ASSIGN(Info);
                    
                };
                
                template <typename T,typename U, typename FUNC>
                struct Ops
                {
                    const Pixmap<U> &source;
                    FUNC            &func;
                    
                    
                    static inline void runMax(Async::Worker &worker,
                                              lockable      &,
                                              void          *data)
                    {
                        assert(data);
                        Ops             &self   = *static_cast<Ops *>(data);
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
                        
                        typedef Info<T> info_type;
                        info_type &result = worker._<info_type>();
                        result.vmax       = vmax;
                        result.pmax       = pmax;
                    }
                    
                    static inline void runMin(Async::Worker &worker,
                                              lockable      &,
                                              void          *data)
                    {
                        assert(data);
                        Ops             &self   = *static_cast<Ops *>(data);
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
                        
                        typedef Info<T> info_type;
                        info_type &result = worker._<info_type>();
                        result.vmin       = vmin;
                        result.pmin       = pmin;
                    }
                    
                };
            };
            
            
            template <typename T, typename U, typename FUNC>
            inline Point FindMax( T &value, const Pixmap<U> &pixmap, FUNC &func, Async::Broker &broker)
            {
                typedef MinMax::Info<T> info_type;
                parallel::group        &cache = *(broker.engine);
                cache.make<info_type>();
                MinMax::Ops<T,U,FUNC> op = { pixmap, func };
                value = 0;
                broker(op.runMax,&op);
                return info_type::FindMax(value,cache);
            }
            
            template <typename T, typename U, typename FUNC>
            inline Point FindMin( T &value, const Pixmap<U> &pixmap, FUNC &func, Async::Broker &broker)
            {
                typedef MinMax::Info<T> info_type;
                parallel::group        &cache = *(broker.engine);
                cache.make<info_type>();
                MinMax::Ops<T,U,FUNC> op = { pixmap, func };
                value = 0;
                broker(op.runMin,&op);
                return info_type::FindMin(value,cache);
            }
            
        }
        
    }
}
#endif

