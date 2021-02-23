
//! \file

#ifndef Y_CONCURRENT_TASK_WORKER_INCLUDED
#define Y_CONCURRENT_TASK_WORKER_INCLUDED 1

#include "y/concurrent/task/types.hpp"
#include "y/concurrent/executable.hpp"
#include "y/concurrent/sync/condition.hpp"
#include "y/core/inode.hpp"

namespace upsylon
{
    namespace concurrent
    {

        class worker :
        public object,
        public inode<worker>,
        public condition,
        public executable::launcher
        {
        public:
            explicit worker(executable  &user_host,  //|
                            const size_t user_size,  //|
                            const size_t user_rank);

            virtual ~worker() throw();




        private:
            Y_DISABLE_COPY_AND_ASSIGN(worker);

        };

    }

}

#endif
