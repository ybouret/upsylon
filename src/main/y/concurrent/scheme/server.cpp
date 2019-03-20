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

        sequential_server:: sequential_server() throw(): server()
        {

        }

        job_uuid sequential_server:: enqueue(const job_type &job)
        {
            ((job_type&)job)(context,access);
            return ++uuid;
        }

        bool sequential_server:: is_done(const job_uuid ) const throw() { return true; }
        bool sequential_server:: is_live(const job_uuid ) const throw() { return false; }

    }
}





