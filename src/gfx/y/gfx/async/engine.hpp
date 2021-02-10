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
            //! alias for Sequential/Parallel server
            //__________________________________________________________________
            typedef arc_ptr<concurrent::server> Server;

            //__________________________________________________________________
            //
            //! Engine to dispatch inner tiles and outer boundaries work
            //__________________________________________________________________
            class Engine : public Tiling
            {
            public:
                //! setup tiles from topology, and create workers
                explicit Engine(const Tiling &tiling);

                //! cleanup
                virtual ~Engine() throw();

                //! load all tasks on the server, then flush
                void cycle(concurrent::server &server);
                
                


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Engine);
                void releaseAll() throw();

                void            *impl;   //!< tasks
                Worker          *worker; //!< workers [0..size-1]
                size_t           wBuilt; //!< built workers
                size_t           wCount; //!< memory
                size_t           wBytes; //!< memory
            };

        }
    }
}

#endif

