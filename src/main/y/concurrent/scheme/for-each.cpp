
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

        size_t for_each:: size() const throw()
        {
            return (**this).size();
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

        const executor & sequential_for:: bulk() const throw() { return engine_; }

        
    }
}
