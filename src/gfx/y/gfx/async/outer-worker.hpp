
//! \file

#ifndef Y_GFX_ASYNC_OUTER_WORKER_INCLUDED
#define Y_GFX_ASYNC_OUTER_WORKER_INCLUDED 1

#include "y/gfx/area/boundaries.hpp"
#include "y/parallel.hpp"

namespace upsylon
{
    namespace GFX
    {
        namespace Async
        {
            //__________________________________________________________________
            //
            //! working on outer boundaries
            //__________________________________________________________________
            class OuterWorker
            {
            public:
                explicit OuterWorker(const Boundaries &); //!< setup
                virtual ~OuterWorker() throw();           //!< cleanup
                virtual void run(parallel&,lockable&);    //!< entry point

                const Boundaries &boundaries;             //!< working boundaries

            private:
                Y_DISABLE_COPY_AND_ASSIGN(OuterWorker);
            };

            
        }
    }
}

#endif

