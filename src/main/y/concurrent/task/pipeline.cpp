
#include "y/concurrent/task/pipeline.hpp"
#include "y/type/self-destruct.hpp"
#include "y/type/aliasing.hpp"
#include "y/type/utils.hpp"
#include <iomanip>

namespace upsylon
{
    namespace concurrent
    {


        static const char pfx[] = "[pipe.";

        pipeline:: ~pipeline() throw()
        {
            cleanup();
        }



        pipeline:: pipeline() :
        executable(),
        waiting(),
        primary(*this, topo->size(), topo->primary_rank() ),
        ready(0),
        built(false),
        verbose( nucleus::thread::verbosity(Y_VERBOSE_THREADS) )
        {
            setup();
        }



        void pipeline:: call(const context &ctx) throw()
        {
            static const char sfx[] = " (primary)";

            access.lock();
            ++ready;
            Y_PIPELINE_LN(pfx<<"call] @" << ctx.label << sfx);

            // wait on LOCKED mutex
            activity.wait(access);

            // wake on LOCKED mutex
            Y_PIPELINE_LN(pfx<<"bip!] @" << ctx.label << sfx);


            // return
            --ready;
            access.unlock();

        }


        void pipeline:: setup()
        {
            try
            {
                Y_PIPELINE_LN(pfx<<"init]");
                const size_t  count = topo->size();
                const size_t &rank  = waiting.size;
                while(rank<count)
                {
                    waiting.push_back( new drone(*this,count,rank) );
                }

                Y_MUTEX_PROBE(access,ready>count);
                Y_PIPELINE_LN(pfx<<"sync]");


                built = true;

            }
            catch(...)
            {
                finish();
                throw;
            }
        }

        void pipeline:: finish()  throw()
        {
            activity.broadcast();
            for(drone *d=waiting.head;d;d=d->next)
            {
                d->broadcast();
            }
            Y_MUTEX_PROBE(access,ready<=0);
        }


        void pipeline:: cleanup() throw()
        {

            access.lock();
            Y_PIPELINE_LN(pfx<<"quit]");

            // cleanup
            cue.remove_pending();
            
            access.unlock();
            
            // flush


            // end
            finish();
        }


        void pipeline:: load1() throw()
        {
            assert(waiting.size>0);
            assert(cue.pending.size>0);
            drone *d = running.push_back( waiting.pop_front() );
            d->deal  = cue.pending.pop_front();
            d->broadcast();
        }




        job::uuid pipeline:: yield(const job::type &J)
        {
            Y_LOCK(access);

            //------------------------------------------------------------------
            //
            // create the job
            //
            //------------------------------------------------------------------
            const job::uuid ans = jid;
            cue.establish(ans,J);
            ++jid;
            
            if(waiting.size)
            {
                load1();
            }
            
            return ans;
        }

        void pipeline:: flush() throw()
        {
            Y_LOCK(access);
        }



    }

}


