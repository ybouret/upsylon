
#include "y/concurrent/task/pipeline.hpp"

namespace upsylon
{
    namespace concurrent
    {

        static const char pfx[] = "[pipeline.";

        pipeline:: ~pipeline() throw()
        {
            Y_PIPELINE_LN(pfx<<"quit] #" << topo->size() );

        }



        pipeline:: pipeline() :
        executable(),
        waiting(),
        working(),
        verbose( nucleus::thread::verbosity(Y_VERBOSE_PIPELINE))
        {
            Y_PIPELINE_LN(pfx<<"init] #" << topo->size() );
        }


        void pipeline:: setup()
        {
            const size_t count = topo->size();
            try
            {
                while(waiting.size<count)
                {
                    break;
                }
            }
            catch(...)
            {
                throw;
            }
        }


        void pipeline:: call(const context & ) throw()
        {
            access.lock();

            access.unlock();
        }
    }

}


