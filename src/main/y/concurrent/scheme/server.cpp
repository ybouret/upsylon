#include "y/concurrent/scheme/server.hpp"

namespace upsylon
{
    namespace concurrent
    {
        server::~server() throw()
        {
        }

        server:: server() throw() : uuid(0)
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

    }
}





