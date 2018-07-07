
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
        context(),
        my_lock()
        {
        }

        void sequential_for:: finish() throw()
        {
            // do nothing
        }

        void sequential_for:: start(kernel proc, void *data)
        {
            assert(proc);
            proc(data,context,my_lock);
        }
    }
}
