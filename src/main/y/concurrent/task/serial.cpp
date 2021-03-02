#include "y/concurrent/task/serial.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon
{
    namespace concurrent
    {

        serial:: ~serial() throw()
        {
        }
        
        serial:: serial() throw() : access()
        {
        }

        job::uuid serial:: yield( const job::type &J )
        {
            aliasing::_(J)(access);
            return ++jid;
        }

        void serial:: flush() throw()
        {
        }
        
        void serial:: batch(addressable<job::uuid> &jids, const accessible<job::type> &jobs)
        {
            assert(jids.size()==jobs.size());
            for(size_t i=jobs.size();i>0;--i)
            {
                jids[i] = yield(jobs[i]);
            }
        }
        
    }
}
