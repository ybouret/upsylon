
#include "y/concurrent/task/pipeline.hpp"
#include "y/type/collapse.hpp"
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

            access.lock();
            Y_PIPELINE_LN(pfx<<"quit] #" << topo->size() );
            todo.release();
            access.unlock();

            finish();
        }



        pipeline:: pipeline() :
        executable(),
        crew(),
        todo(),
        done(),
        ready(0),
        verbose( nucleus::thread::verbosity(Y_VERBOSE_THREADS) )
        {
            setup();
        }


        void pipeline:: finish() throw()
        {
            for(worker *w=crew.head;w;w=w->next)
            {
                w->broadcast();
            }

            Y_MUTEX_PROBE(access,ready<=0);
        }


        void pipeline:: setup()  
        {
            Y_PIPELINE_LN(pfx<<"make] <#" << topo->size() << ">  --------");
            const size_t  count = topo->size();
            const size_t &rank  = crew.size;
            try
            {
                while(rank<count)
                {
                    crew.push_back( new worker(*this,count,rank) );
                }
                Y_MUTEX_PROBE(access,ready>=count);

            }
            catch(...)
            {
                finish();
                throw;
            }

            Y_PIPELINE_LN(pfx<<"made] <#" << topo->size() << "/> --------" );


        }



        void pipeline:: call(const context &ctx) throw()
        {

            access.lock();
            Y_PIPELINE_LN(pfx<<"init] @" << ctx.label);

            
            access.unlock();

        }

        job::uuid pipeline:: yield(const job::type &J)
        {
            Y_LOCK(access);

            //------------------------------------------------------------------
            //
            // create the job
            //
            //------------------------------------------------------------------
            todo.append(jid,J,done);
            ++jid;
            Y_PIPELINE_LN(pfx<<"+job] $" << todo.tail->uuid);


            return 0;
        }

        void pipeline:: flush() throw()
        {
            Y_LOCK(access);

        }



    }

}


