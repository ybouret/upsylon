//! \file

#ifndef Y_GFX_ASYNC_ENGINE_INCLUDED
#define Y_GFX_ASYNC_ENGINE_INCLUDED 1


#include "y/gfx/area/tiles.hpp"
#include "y/concurrent/scheme/server.hpp"
#include "y/sequence/vector.hpp"

namespace upsylon
{
    namespace GFX
    {
        namespace Async
        {
            typedef arc_ptr<concurrent::server> Server;

            class Worker
            {
            public:
                virtual ~Worker() throw();
                virtual  void compute(parallel&,lockable&) = 0;

            protected:
                explicit Worker();
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Worker);
            };

            class InnerWorker : public Worker
            {
            public:
                virtual ~InnerWorker() throw();
                explicit InnerWorker(const Tile &);
                virtual  void compute(parallel&,lockable&);

                const Tile &tile;



            private:
                Y_DISABLE_ASSIGN(InnerWorker);
            };

            class OuterWorker : public Worker
            {
            public:
                explicit OuterWorker(const Boundaries &);
                virtual ~OuterWorker() throw();
                virtual  void compute(parallel&,lockable&);

                const Boundaries &boundaries;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(OuterWorker);
            };

            class Engine : public Tiles
            {
            public:
                explicit Engine(const size_t             cpus,
                                const Topology::Pointer &topo);
                virtual ~Engine() throw();


                void cycle(concurrent::server &server);


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

