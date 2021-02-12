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
           
            typedef parallel::group Team;
            
            //__________________________________________________________________
            //
            //
            //! Engine to dispatch inner tiles and outer boundaries work
            //
            //__________________________________________________________________
            class Engine : public Object, public Tiles, public Team
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
                // addressable<parallel>
                //______________________________________________________________
                virtual size_t size() const throw(); //!< count=number of tiles
                virtual parallel       & operator[](const size_t)       throw(); //!< [1..size]
                virtual const parallel & operator[](const size_t) const throw(); //!< [1..size]

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________

                //! load all tasks on the server, then flush
                void cycle(concurrent::server &server,
                           Worker::Routine     kproc,
                           void               *kdata);
                


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Engine);
                void releaseAll() throw();

                void            *impl;   //!< tasks
                Worker          *worker; //!< workers [0..size-1]
                Worker          *wShift; //!< workers [1..size];
                size_t           wBuilt; //!< built workers
                size_t           wCount; //!< memory
                size_t           wBytes; //!< memory
            };

            typedef arc_ptr<Engine> SharedEngine; //!< alias

        }
    }
}

#endif

