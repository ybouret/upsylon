//! \file

#ifndef Y_GFX_ASYNC_ENGINE_INCLUDED
#define Y_GFX_ASYNC_ENGINE_INCLUDED 1


#include "y/gfx/area/tiles.hpp"
#include "y/gfx/async/inner-worker.hpp"
#include "y/gfx/async/outer-worker.hpp"
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
            class Engine : public Tiles
            {
            public:
                //! setup tiles from topology, and create workers
                explicit Engine(const size_t             cpus,
                                const Topology::Pointer &topo);

                //! cleanup
                virtual ~Engine() throw();

                //! load all tasks on the server, then flush
                void cycle(concurrent::server &server);
                

                //! cache setup
                template <typename T>
                void cache(size_t n)
                {
                    outsideWorker.make<T>(n);
                    for(size_t i=0;i<iwBuilt;++i)
                    {
                        insideWorkers[i].make<T>(n);
                    }
                }

            protected:
                void            *impl;          //!< tasks
                OuterWorker      outsideWorker; //!< 1 for boundaries
                InnerWorker     *insideWorkers; //!< min_of(size,inner)
                size_t           iwBuilt;       //!< built insideWorkers
                size_t           iwCount;       //!< allocated insideWorkers
                size_t           iwBytes;       //!< bytes for insideWorkers

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Engine);
                void releaseAll() throw();
            };

        }
    }
}

#endif

