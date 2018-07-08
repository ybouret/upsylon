
#include "y/concurrent/scheme/for-each.hpp"

namespace upsylon
{
    namespace concurrent
    {
        for_each:: ~for_each() throw()
        {
        }

        for_each:: for_each() throw()
        {
        }

    }
}

namespace upsylon
{
    namespace concurrent
    {
        sequential_for:: ~sequential_for() throw()
        {
        }

        sequential_for:: sequential_for() throw() :
        for_each(),
        engine_()
        {
        }

        void sequential_for:: finish() throw()
        {
            // do nothing
        }

        void sequential_for:: start(kernel proc, void *data)
        {
            assert(proc);
            engine_.run(proc,data);
        }
    }
}
