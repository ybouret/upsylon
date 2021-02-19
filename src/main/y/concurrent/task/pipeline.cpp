
#include "y/concurrent/task/pipeline.hpp"
#include <iomanip>

namespace upsylon
{
    namespace concurrent
    {

        pipeline:: engine:: engine(executable   &user_host,
                                   const size_t  user_size,
                                   const size_t  user_rank) :
        object(),
        executable::launcher(user_host,user_size,user_rank),
        inode<engine>()
        {
        }


        pipeline:: engine:: ~engine() throw()
        {
        }


        static const char pfx[] = "[pipe.";

        pipeline:: ~pipeline() throw()
        {
            cleanup();
        }



        pipeline:: pipeline() :
        executable(),
        waiting(),
        working(),
        ready(0),
        start(),
        built(false),
        verbose( nucleus::thread::verbosity(Y_VERBOSE_PIPELINE) )
        {
            setup();
        }

        void pipeline:: cleanup() throw()
        {
            Y_PIPELINE_LN(pfx<<"quit] #" << topo->size() );
            start.broadcast();
        }

        void pipeline:: setup()
        {
            const size_t count = topo->size();
            try
            {
                // building threads
                {
                    Y_PIPELINE_LN(pfx<<"init] #" << topo->size() );
                    size_t rank = 0;
                    while(rank<count)
                    {
                        engine *eng = new engine(*this,count,rank);
                        waiting.push_back(eng);
                        Y_MUTEX_PROBE(access,ready>rank);
                        ++rank;
                    }
                    Y_PIPELINE_LN(pfx<<"made] #" << topo->size() );
                }

                // thread placement
                {
                    assert(waiting.size==topo->nodes.size);
                    const topology::node *cpu = topo->nodes.head;
                    engine               *eng = waiting.head;
                    for(size_t n=count;n>0;--n,cpu=cpu->next,eng=eng->next)
                    {
                        const char *who = verbose ? eng->label : NULL;
                        nucleus::thread::assign(eng->handle,cpu->rank,who);
                    }
                }

                // all built
                built = true;

            }
            catch(...)
            {
                start.broadcast();
                Y_MUTEX_PROBE(access,ready<=0);
                throw;
            }
        }


        void pipeline:: call(const context &ctx) throw()
        {
            // LOCK
            access.lock();
            ++ready;
            Y_PIPELINE_LN(pfx<<"<ok>] @ " << ctx.label << " (ready = " << std::setw(ctx.setw) << ready << "/" << ctx.size << ")" );

            // wait on a LOCKED mutex
            start.wait(access);

            // awake on a LOCKED mutex
            if(!built)
            {
                // emergency exit
                Y_PIPELINE_LN(pfx<<"err!] @"<<ctx.label);
                --ready;
                access.unlock();
                return;
            }

            // first valid wake up :)



            access.unlock();
        }
    }

}


