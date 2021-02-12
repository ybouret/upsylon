
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
            //! alias to use in parallel routines
            //__________________________________________________________________
            typedef parallel_cache Cache;
            
            //__________________________________________________________________
            //
            //
            //! worker interface, with local cache
            //
            //__________________________________________________________________
            class Worker : public parallel
            {
            public:
                //! runnable routine
                typedef void (*Routine)(Worker &,lockable &,void *);

                explicit Worker(const size_t sz, const size_t rk, const Tile &); //!< setup
                virtual ~Worker() throw();     //!< cleanup
                virtual void run(lockable &);//!< entry point
                static  void None(Worker &,lockable &,void*); //!< default kernel, display tile info

                const Tile  &tile;                  //!< working tile
                Point        begin() const throw(); //!< first point

                //! set internal kernel
                void load(Routine,void*) throw();
                

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Worker);
                Routine kproc;
                void   *kdata;

            };

        }

    }

}

#endif
