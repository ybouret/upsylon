
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
            //
            //! worker interface, with local cache
            //
            //__________________________________________________________________
            class Worker : public parallel
            {
            public:
                typedef void (*Kernel)(Worker &,lockable &,void *);

                explicit Worker(const size_t sz, const size_t rk, const Tile &); //!< setup
                virtual ~Worker() throw();     //!< cleanup
                virtual void run(lockable &);//!< entry point


                const Tile  &tile; //!< working tile

                Kernel kproc;
                void  *kdata;


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Worker);
            };

        }

    }

}

#endif
