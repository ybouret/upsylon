#include "y/concurrent/task/worker.hpp"

namespace upsylon
{
    namespace concurrent
    {
        worker:: ~worker() throw()
        {
        }

    }
}

#include "y/concurrent/task/pipeline.hpp"

namespace upsylon
{
    namespace concurrent
    {

        worker:: worker(pipeline    &user_pipe,
                        const size_t user_size,
                        const size_t user_rank) :
        authority<pipeline>(user_pipe),
        condition(),
        thread(stub,this,user_size,user_rank)
        {

        }

        void worker:: stub(void *addr) throw()
        {
            assert(addr);
            static_cast<worker *>(addr)->loop();
        }

        static const char pfx[] = "[work.";

        void worker:: loop() throw()
        {
            pipeline   &self    = **this;
            const bool  verbose = self.verbose;
            mutex      &access  = self.access;
            size_t     &ready   = self.ready;
            
            // LOCK access
            access.lock();
            Y_PIPELINE_LN(pfx<<"init] @"<<label);
            ++ready;
            
            // waiting on a LOCKED mutex
            wait(access);

            Y_PIPELINE_LN(pfx<<"done] @"<<label);
            --ready;
            access.unlock();
        }

    }
}
