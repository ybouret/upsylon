
//! \file

#ifndef Y_GFX_ASYNC_WORKER_INCLUDED
#define Y_GFX_ASYNC_WORKER_INCLUDED 1

#include "y/parallel.hpp"

namespace upsylon
{
    namespace GFX
    {
        namespace Async
        {
            //__________________________________________________________________
            //
            //! Cache for local computations
            //__________________________________________________________________
            typedef memory::shack Cache;

            //__________________________________________________________________
            //
            //! worker interface, with local cache
            //__________________________________________________________________
            class Worker : public Cache
            {
            public:
                virtual ~Worker() throw(); //!< cleanup
                virtual void run(parallel &, lockable &) = 0; //!< entry point

            protected:
                explicit Worker(); //!< setup

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Worker);
            };

        }

    }

}

#endif
