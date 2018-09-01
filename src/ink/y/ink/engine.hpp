//! \file
#ifndef Y_INK_ENGINE_INCLUDED
#define Y_INK_ENGINE_INCLUDED 1

#include "y/concurrent/scheme/simd.hpp"
#include "y/ink/tiles.hpp"
#include "y/ptr/arc.hpp"

namespace upsylon
{
    namespace Ink
    {
        //! shared for_each object
        typedef arc_ptr<concurrent::for_each> Dispatcher;


        //! parallel engine for a given area
        /**
         build an engine for a given area, with a reusable dispatcher
         */
        class Engine : public Area
        {
        public:
            //! prototype for the use of functor
            typedef functor<void,TL2(const Tile &,lockable &)> Function;

            //! constructor, build tiles with shared from full
            explicit Engine(const Dispatcher &shared,
                            const Area       &full);
            //! desctructor
            virtual ~Engine() throw();


            //! FUNC(zone,sync), generic parallel processing
            template <typename FUNC>
            void run( FUNC &func )
            {
                parameters<FUNC> params = { this, &func };
                agent->run( call<FUNC>, &params);
            }

            //! local caches
            void acquire_all( const size_t n );

            //! get min of all local cache
            template <typename T>
            inline T get_min() const
            {
                assert(tiles.size()>0);
                size_t n = tiles.size();
                T ans = tiles[n]->cache.get<T>();
                for(--n;n>0;--n)
                {
                    const T tmp = tiles[n]->cache.get<T>();
                    if(tmp<ans)
                    {
                        ans = tmp;
                    }
                }
                return ans;
            }

            //! get max of all local cache
            template <typename T>
            inline T get_max() const
            {
                assert(tiles.size()>0);
                size_t n = tiles.size();
                T ans = tiles[n]->cache.get<T>();
                for(--n;n>0;--n)
                {
                    const T tmp = tiles[n]->cache.get<T>();
                    if(tmp>ans)
                    {
                        ans = tmp;
                    }
                }
                return ans;
            }

            //! get min/max of all local cache
            template <typename T>
            inline void get_min_max( T &vmin, T &vmax) const
            {
                assert(tiles.size()>0);
                size_t n = tiles.size();
                vmin = vmax  = tiles[n]->cache.get<T>();
                for(--n;n>0;--n)
                {
                    {
                        const T tmp = tiles[n]->cache.get<T>(0);
                        if(tmp<vmin)
                        {
                            vmin = tmp;
                        }
                    }
                    {
                        const T tmp = tiles[n]->cache.get<T>(1);
                        if(tmp>vmax)
                        {
                            vmax = tmp;
                        }
                    }
                }
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Engine);
            
            Dispatcher  agent;
            template <typename FUNC>
            struct parameters
            {
                Engine *self_p;
                FUNC   *func_p;
            };

            template <typename FUNC>
            static inline void call( void *addr, parallel &ctx, lockable &sync )
            {
                assert(addr);
                parameters<FUNC> *p = static_cast<parameters<FUNC>*>(addr);
                assert(p->self_p);
                assert(p->func_p);
                assert(ctx.size==p->self_p->tiles.size());

                Engine     &self = *(p->self_p);
                FUNC       &func = *(p->func_p);
                const Tile &tile = *self.tiles[ctx.indx];
                if(tile.pixels)
                {
                    func(tile,sync);
                }
            }

        public:
            const Tiles tiles; //!< computed tiles
        };

    }
}


#endif
