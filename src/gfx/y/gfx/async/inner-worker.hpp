//! \file

#ifndef Y_GFX_ASYNC_INNER_WORKER_INCLUDED
#define Y_GFX_ASYNC_INNER_WORKER_INCLUDED 1

#include "y/gfx/area/tile.hpp"
#include "y/gfx/async/worker.hpp"

namespace upsylon
{
    namespace GFX
    {
        namespace Async
        {
            //__________________________________________________________________
            //
            //! worker on an inner tile
            //__________________________________________________________________
            class InnerWorker : public Worker
            {
            public:
                virtual ~InnerWorker() throw();         //!< cleanup
                explicit InnerWorker(const Tile &);     //!< setup
                virtual  void run(parallel&,lockable&); //!< main call

                const Tile &tile;                       //!< working tile
                
             private:
                Y_DISABLE_ASSIGN(InnerWorker);
            };
        }

    }

}

#endif


