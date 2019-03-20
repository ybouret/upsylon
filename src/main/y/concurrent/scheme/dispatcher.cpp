
#include "y/concurrent/scheme/dispatcher.hpp"

namespace upsylon
{
    namespace concurrent
    {
        dispatcher:: ~dispatcher() throw()
        {
        }


        dispatcher:: dispatcher(const bool v) :
        workers(v),
        verbose( workers.verbose )
        {
        }
    }
}
