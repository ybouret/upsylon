#include "y/concurrent/scheme/server.hpp"

namespace upsylon
{
    namespace concurrent
    {
        server::~server() throw()
        {
        }

        server:: server() throw() : uuid(1)
        {}


    }

}

namespace upsylon
{
    namespace concurrent
    {
        sequential_server:: ~sequential_server() throw() {}

        sequential_server:: sequential_server() throw():
        server(), impl()
        {

        }

        job_uuid sequential_server:: enqueue(const job_type &job)
        {
            ((job_type&)job)(impl[0],impl.access);
            return ++uuid;
        }

        void sequential_server:: flush() throw() {}

        executor & sequential_server:: engine() throw()
        {
            return impl;
        }

        void sequential_server:: process(job_uuids       &uuids,
                                         const job_batch &batch)
        {
            assert(uuids.size()==batch.size());
            const size_t n = uuids.size();
            for(size_t i=n;i>0;--i)  uuids[i] = 0;
            for(size_t i=1;i<=n;++i) uuids[i] = enqueue(batch[i]);
        }

    }
}





