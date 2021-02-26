#include "y/concurrent/task/worker.hpp"

namespace upsylon
{
    namespace concurrent
    {
        worker:: ~worker() throw()
        {
        }

        worker:: worker(pipeline    &user_pipe,
                        const size_t user_size,
                        const size_t user_rank) :
        authority<pipeline>(user_pipe),
        condition(),
        thread(stub,this,user_size,user_rank),
        deal(NULL)
        {

        }
    }
}

#include "y/concurrent/task/pipeline.hpp"

namespace upsylon
{
    namespace concurrent
    {

        void worker:: stub(void *addr) throw()
        {
            assert(addr);
            worker   *replica = static_cast<worker *>(addr);
            pipeline &primary = **replica;
            primary.loop(replica);
        }
        
    }
}
