#include "y/concurrent/scheme/server.hpp"

namespace upsylon
{
    namespace concurrent
    {

        server:: server() throw() :
        jobs(),
        jmem(),
        uuid(0)
        {
        }
        

        server:: ~server() throw()
        {
            while( jobs.size )
            {
                jnode *j = jobs.pop_back();
                destruct(j);
                object::release1(j);
            }

            while( jmem.size )
            {
                jnode *j = jmem.query();
                object::release1(j);
            }


        }

        job_uuid server:: append(const job_type &J)
        {
            jnode *j = (jmem.size>0) ? jmem.query() : object::acquire1<jnode>();
            try
            {
                const job_uuid next_uuid = uuid+1;
                jobs.push_back( new (j) jnode(next_uuid,J) );
                uuid = next_uuid;
                return j->uuid;
            }
            catch(...)
            {
                jmem.store(j);
                throw;
            }
        }

        void server:: remove(jnode *j) throw()
        {
            assert(j);
            assert(j->next==0);
            assert(j->prev==0);
            destruct(j);
            jmem.store(j);
        }

        void server:: remove_pending_jobs() throw()
        {
            while( jobs.size ) remove( jobs.pop_back() );
        }
        
    }
}

