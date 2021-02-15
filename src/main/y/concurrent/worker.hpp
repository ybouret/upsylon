//! \file
#ifndef Y_THREAD_WORKER_INCLUDED
#define Y_THREAD_WORKER_INCLUDED 1

#include "y/concurrent/nucleus.hpp"
#include "y/concurrent/context.hpp"

namespace upsylon {

    namespace concurrent
    {

        class worker : public context
        {
        public:
            virtual ~worker() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(worker);
        };


    }

}

#endif
