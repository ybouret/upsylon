//! \file

#ifndef Y_GFX_ASYNC_ENGINE_INCLUDED
#define Y_GFX_ASYNC_ENGINE_INCLUDED 1


#include "y/gfx/area/tiles.hpp"
#include "y/gfx/async/worker.hpp"
#include "y/concurrent/scheme/server.hpp"

namespace upsylon
{
    namespace GFX
    {
        namespace Async
        {
           
            //__________________________________________________________________
            //
            //
            //! Engine to dispatch inner tiles and outer boundaries work
            //
            //__________________________________________________________________
            class Engine : public Object, public Tiles
            {
            public:
                //______________________________________________________________
                //
                // C++
                //__________________________________________________________________
                //! setup tiles from topology, and create workers
                explicit Engine(const Area &area, const size_t maxThreads);

                //! cleanup
                virtual ~Engine() throw();

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________

                //! load all tasks on the server, then flush
                void cycle(concurrent::server &server);

                //! prepare cache for workers
                template <typename T> inline
                void cache(const size_t n)
                {
                    for(size_t i=0;i<size;++i)
                    {
                        worker[i].make<T>(n);
                    }
                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Engine);
                void releaseAll() throw();

                void            *impl;   //!< tasks
                Worker          *worker; //!< workers [0..size-1]
                size_t           wBuilt; //!< built workers
                size_t           wCount; //!< memory
                size_t           wBytes; //!< memory
            };

            typedef arc_ptr<Engine> Tiling;

        }
    }
}

#endif

