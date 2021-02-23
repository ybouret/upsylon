
#include "y/concurrent/task/worker.hpp"

namespace upsylon
{
    namespace concurrent
    {

        worker:: ~worker() throw()
        {

        }

        worker:: worker(executable  &user_host,
                        const size_t user_size,
                        const size_t user_rank) :
        condition(),
        executable::launcher(user_host, user_size, user_rank)
        {

        }

    }
}
