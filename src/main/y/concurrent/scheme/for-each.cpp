
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

        size_t for_each:: number() const throw()
        {
            return engine().num_threads();
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


        void sequential_for:: run(kernel proc, void *data)
        {
            assert(proc);
            engine_.run(proc,data);
        }

        executor & sequential_for:: engine() throw() { return engine_; }

        const executor & sequential_for:: engine() const throw() { return engine_; }

    }
}
