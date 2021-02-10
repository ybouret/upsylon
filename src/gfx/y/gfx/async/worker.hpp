
//! \file

#ifndef Y_GFX_ASYNC_WORKER_INCLUDED
#define Y_GFX_ASYNC_WORKER_INCLUDED 1

#include "y/parallel.hpp"
#include "y/gfx/area/tile.hpp"

namespace upsylon
{
    namespace GFX
    {
        namespace Async
        {
            //__________________________________________________________________
            //
            //! worker interface, with local cache
            //__________________________________________________________________
            class Worker : public parallel
            {
            public:
                explicit Worker(const size_t sz, const Tile &); //!< setup
                virtual ~Worker() throw();     //!< cleanup
                virtual void run(lockable &);//!< entry point
                const Tile &tile; //!< working tile
                
                

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Worker);
            };

        }

    }

}

#endif
