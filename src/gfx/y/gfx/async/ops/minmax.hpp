//! \file

#ifndef Y_GFX_ASYNC_MINMAX_INCLUDED
#define Y_GFX_ASYNC_MINMAX_INCLUDED 1

#include "y/gfx/async/broker.hpp"


namespace upsylon
{
    namespace GFX
    {
        template <typename T> class Pixmap; //!< forward declaration
        
        namespace Async
        {

            //__________________________________________________________________
            //
            //
            //! Find Min/Max on a pixmap
            //
            //__________________________________________________________________
            struct MinMax
            {

                //______________________________________________________________
                //
                //! info to locate min/max
                //______________________________________________________________
                template <typename T>
                class Info
                {
                public:

                    inline  Info() throw() : vmax(0), pmax(), vmin(0), pmin() {} //!< setup
                    inline ~Info() throw() {}                                    //!< cleanup

                    T      vmax; //!< max value
                    Point  pmax; //!< max position
                    T      vmin; //!< min value
                    Point  pmin; //!< min position

                    //! Reduction after FindMin per Tile
                    static inline Point FindMinReduce(T &value, const accessible<parallel> &arr)
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

                    //! Reduction after FindMax per Tile
                    static inline Point FindMaxReduce(T &value, const accessible<parallel> &arr)
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

                    //! Reduction after FindMinMax per Tile
                    static inline void FindMinMaxReduce(Info &result, const accessible<parallel> &arr)
                    {
                        const Info *info = & arr[1]._<Info>();
                        Point       pmax = info->pmax;
                        T           vmax = info->vmax;
                        Point       pmin = info->pmin;
                        T           vmin = info->vmin;
                        
                        for(size_t i=arr.size();i>1;--i)
                        {
                            info = & arr[i]._<Info>();
                            {
                                const T temp = info->vmin;
                                if(temp<vmin)
                                {
                                    vmin = temp;
                                    pmin = info->pmin;
                                }
                            }
                            {
                                const T temp = info->vmax;
                                if(temp>vmax)
                                {
                                    vmax = temp;
                                    pmax = info->pmax;
                                }
                            }
                            
                        }
                        result.vmax = vmax;
                        result.pmax = pmax;
                        
                        result.vmin = vmin;
                        result.pmin = pmin;
                    }
                    
                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Info);
                    
                };

                //______________________________________________________________
                //
                //! parallel operations
                //______________________________________________________________
                template <typename T,typename U, typename FUNC>
                struct Ops
                {
                    const Pixmap<U> &source; //!< mapped source
                    FUNC            &func;   //!< function per pixel
                    

                    //! find max per tile, store in cache
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
                        
                        typedef Info<T> InfoType;
                        InfoType  &result = worker._<InfoType>();
                        result.vmax       = vmax;
                        result.pmax       = pmax;
                    }

                    //! find min per tile, store in cache
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
                        
                        typedef Info<T> InfoType;
                        InfoType &result = worker._<InfoType>();
                        result.vmin      = vmin;
                        result.pmin      = pmin;
                    }

                    //! find min/max per tile, store in cache
                    static inline void runMinMax(Async::Worker &worker,
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
                        Point            pmax   = pmin;
                        T                vmax   = vmin;
                        
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
                                else
                                {
                                    if(temp>vmax)
                                    {
                                        vmax = temp;
                                        pmax = p;
                                    }
                                }
                            }
                        }
                        
                        typedef Info<T> InfoType;
                        InfoType &result = worker._<InfoType>();
                        result.vmin      = vmin;
                        result.pmin      = pmin;
                        result.vmax      = vmax;
                        result.pmax      = pmax;
                    }
                    
                };
            };
            

            //__________________________________________________________________
            //
            //! Find Max of func(pixmap)
            //__________________________________________________________________
            template <typename T, typename U, typename FUNC>
            inline Point FindMax( T &value, const Pixmap<U> &pixmap, FUNC &func, Async::Broker &broker)
            {
                typedef MinMax::Info<T> InfoType;
                Team                   &cache = *broker; cache.make<InfoType>();
                MinMax::Ops<T,U,FUNC>   op    = { pixmap, func };
                broker(op.runMax,&op);
                return InfoType::FindMaxReduce(value,cache);
            }

            //__________________________________________________________________
            //
            //! Find Min of func(pixmap)
            //__________________________________________________________________
            template <typename T, typename U, typename FUNC>
            inline Point FindMin( T &value, const Pixmap<U> &pixmap, FUNC &func, Async::Broker &broker)
            {
                typedef MinMax::Info<T> InfoType;
                parallel::group        &cache = *broker; cache.make<InfoType>();
                MinMax::Ops<T,U,FUNC>   op = { pixmap, func };
                broker(op.runMin,&op);
                return InfoType::FindMinReduce(value,cache);
            }

            //__________________________________________________________________
            //
            //! Find Min/Max of func(pixmap)
            //__________________________________________________________________
            template <typename T, typename U, typename FUNC>
            inline void FindMinMax( MinMax::Info<T> &info, const Pixmap<U> &pixmap, FUNC &func, Async::Broker &broker)
            {
                typedef MinMax::Info<T> InfoType;
                parallel::group        &cache = *(broker.engine); cache.make<InfoType>();
                MinMax::Ops<T,U,FUNC>   op = { pixmap, func };
                broker(op.runMinMax,&op);
                InfoType::FindMinMaxReduce(info,cache);
            }
            
        }
        
    }
}
#endif
