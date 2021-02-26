
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
        }



        pipeline:: pipeline() :
        executable(),
        verbose( nucleus::thread::verbosity(Y_VERBOSE_THREADS) )
        {
        }


        void pipeline:: setup()  
        {
            
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


