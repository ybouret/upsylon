#include "y/concurrent/task/drone.hpp"

namespace upsylon
{
    namespace concurrent
    {

        drone:: ~drone() throw()
        {
        }

    }

}



#include "y/concurrent/task/pipeline.hpp"

namespace upsylon
{
    namespace concurrent
    {

        static const char pfx[] = "[:drone:";

        drone:: drone(pipeline     &user_pipe,
                      const size_t user_size,
                      const size_t user_rank) :
        object(),
        condition(),
        authority<pipeline>(user_pipe),
        thread(stub,this,user_size,user_rank),
        inode<drone>(),
        deal(NULL)
        {
        }

        void drone:: stub(void *addr) throw()
        {
            assert(addr);
            static_cast<drone*>(addr)->loop();
        }

        void drone:: loop() throw()
        {
            // init
            pipeline   &self    = **this;
            mutex      &access  = self.access;
            const bool &verbose = self.verbose;

            access.lock();
            Y_PIPELINE_LN(pfx<<".+] @" << label);
            ++self.ready;

            // first sync on LOCKED mutex
        LOOP:
            wait(access);

            // wake up on LOCKED mutex
            if(deal)
            {
                Y_PIPELINE_LN(pfx<<".>] @" << label);
                assert(self.running.owns(this));

                // run UNLOCKED
                access.unlock();
                deal->type(access);
                access.lock();

                // done LOCKED
                Y_PIPELINE_LN(pfx<<".<] @" << label);
                self.cue.terminate(deal);
                self.waiting.push_back( self.running.unlink(this) );


                // signal activity
                self.activity.broadcast();

                // and wait for next deal
                goto LOOP;

            }
            else
            {
                Y_PIPELINE_LN(pfx<<".-] @" << label);
            }
            
            

            // return
            --self.ready;
            access.unlock();

        }



    }

}
